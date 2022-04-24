cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
    float3 cameraPos;
};

struct VertexInputType
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPos : POSITION;
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
    
    output.viewDirection = normalize(cameraPos - worldPosition.xyz);
    
    output.worldPos = worldPosition.xyz;
    
    return output;
}