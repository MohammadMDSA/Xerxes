#include "Utility.hlsli"

Texture2D shaderTextures[3];
SamplerState sampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float3 lightDirection;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPos : POSITION;
};

float4 main(PixelInputType pin) : SV_Target
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 specularIntensity;
    float3 reflection;
    float4 specular;
    float3 tangent;
    float3 bitangent;
    float3 normal;
    
    float3x3 tbn;
    tbn = CalculateTBN(pin.worldPos, pin.normal, pin.tex);
    tangent = tbn[0];
    bitangent = tbn[1];
    normal = normalize(mul(pin.normal, tbn));
    
    textureColor = shaderTextures[0].Sample(sampleType, pin.tex);
    
    bumpMap = shaderTextures[1].Sample(sampleType, pin.tex);
    
    bumpMap = (bumpMap * 2.f) - 1.f;
    
    specularIntensity = shaderTextures[2].Sample(sampleType, pin.tex);
    
    float4 ambient = 0.1f * textureColor;
    
    bumpNormal = normal + bumpMap.x * tangent + bumpMap.y * bitangent;
    
    bumpNormal = normalize(bumpNormal);
    
    lightDir = -lightDirection;
    
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    
    color = saturate(diffuseColor * lightIntensity);
    
    color = color * textureColor;
    
    reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);
        
    float3 halfWay = normalize(lightDir + pin.viewDirection);
        
    specular = pow(saturate(dot(bumpNormal, halfWay)), specularPower);
        
    specular = specular * specularIntensity;
        
    color = saturate(color + specular + ambient);
    
    return color;
    //return shaderTextures[1].Sample(sampleType, pin.tex);

}
