cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
};

struct VertexInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDirection : TEXCOORD1;
};

PixelInputType main(VertexInputType vin)
{
    PixelInputType output;
    float4 worldPosition;
    
    vin.position.w = 1.f;
    
    worldPosition = mul(vin.position, world);
    output.position = mul(worldPosition, view);
    output.position = mul(output.position, proj);
    
    output.tex = vin.tex;
    
    output.normal = normalize(mul(vin.normal, (float3x3) world));
    
    output.tangent = normalize(mul(vin.tangent, (float3x3) world));
    
    output.binormal = normalize(mul(vin.binormal, (float3x3) world));
    
    output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
   
    return output;
}