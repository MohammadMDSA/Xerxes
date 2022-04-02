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
    float3 tangetn : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDirection : TEXCOORD1;
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
    
    textureColor = shaderTextures[0].Sample(sampleType, pin.tex);
    
    bumpMap = shaderTextures[1].Sample(sampleType, pin.tex);
    
    bumpMap = (bumpMap * 2.f) - 1.f;
    
    bumpNormal = pin.normal + bumpMap.x * pin.tangetn + bumpMap.y * pin.binormal;
    
    bumpNormal = normalize(bumpNormal);
    
    lightDir = -lightDirection;
    
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    
    color = saturate(diffuseColor * lightIntensity);
    
    color = color * textureColor;
    
    if(lightIntensity > 0.f)
    {
        specularIntensity = shaderTextures[2].Sample(sampleType, pin.tex);
        
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);
        
        specular = pow(saturate(dot(reflection, pin.viewDirection)), specularPower);
        
        specular = specular * specularIntensity;
        
        color = saturate(color + specular);
    }
    
    return color;

}
