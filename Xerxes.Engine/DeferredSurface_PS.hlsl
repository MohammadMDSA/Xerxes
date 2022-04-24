#include "DeferredCommon.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularIntTexture : register(t2);
Texture2D specularPowTexture : register(t3);

SamplerState sampleState: register(s0);

PS_GBUFFER_OUT main(VS_SURFACE_OUT vin)
{
    float3 DiffuseColor;
    float3 Normal;
    float SpecIntensity;
    float SpecPower;
    
    // Reading values from texture
    DiffuseColor = diffuseTexture.Sample(sampleState, vin.uv);
    Normal = normalTexture.Sample(sampleState, vin.uv);
    SpecIntensity = specularIntTexture.Sample(sampleState, vin.uv);
    SpecPower = specularPowTexture.Sample(sampleState, vin.uv);
        
    return PackGBuffer(DiffuseColor, Normal, SpecIntensity, SpecPower);
}