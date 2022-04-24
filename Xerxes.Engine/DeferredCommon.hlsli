#include "Common.hlsli"

Texture2D DepthTexture : register(t5);
Texture2D ColorSpecIntTexture : register(t6);
Texture2D NormalTexture : register(t7);
Texture2D SpecPowTexture : register(t8);

cbuffer CBGBufferUnpack : register(b5)
{
    float4      PerspectiveValues;
    float4x4    ViewInv;
}

static const float2 g_SpecPowerRange = { 0.1, 250.0 };

struct SURFACE_DATA
{
    float LinearDepth;
    float3 Color : Color;
    float3 Normal : NORMAL;
    float SpecInt;
    float SpecPow;
};

struct VS_LIGHT_OUT
{
    float4 Position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct VS_SURFACE_OUT
{
    sample float4 position : SV_Position;
    sample float3 normal : Normal;
    sample float2 uv : TexCoord0;
};

struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt : SV_Target0;
    float4 Normal : SV_Target1;
    float4 SpecPow : SV_Target2;
};

float ConvertPerspectiveDepthToLinear(float depth)
{
    return PerspectiveValues.z / (depth * PerspectiveValues.w);
}

SURFACE_DATA UnpachGBuffer(int2 location)
{
    SURFACE_DATA sout;
    int3 location3 = int3(location, 0);
    
    float depth = DepthTexture.Load(location3).x;
    sout.LinearDepth = ConvertPerspectiveDepthToLinear(depth);
    
    // Get the base color and specular intencity
    float4 baseColorSpecInt = ColorSpecIntTexture.Load(location3);
    sout.Color = baseColorSpecInt.xyz;
    sout.SpecInt = baseColorSpecInt.w;
    
    // Sample the normal, convert it to the full range and normalize it
    sout.Normal = BiasX2(NormalTexture.Load(location3).xyz);
    
    // Scale the specular power back to the original range
    float specPowerNorm = SpecPowTexture.Load(location3).x;
    sout.SpecPow = g_SpecPowerRange.x + specPowerNorm * g_SpecPowerRange.y;
    
    return sout;

}

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float SpecIntencity, float SpecPower)
{
    PS_GBUFFER_OUT gout;
    
    // Normalize the specular power
    float specPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;
    
    // Pack all the data into the GBuffer structure
    gout.ColorSpecInt = float4(BaseColor.rgb, SpecIntencity);
    gout.Normal = float4(BiasD2(Normal.xyz), 0.0);
    gout.SpecPow = float4(specPowerNorm, 0.0, 0.0, 0.0);
    
    return gout;
}