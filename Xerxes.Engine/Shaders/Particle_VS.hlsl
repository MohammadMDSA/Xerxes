// GLOBALS //
cbuffer MatrixBuffer
{
	matrix WorldViewProj;
	matrix IViewRot;
	matrix IRot;
};


// TYPEDEFS //

struct VertexInputType
{
	float4 position : SV_POSITION;
	float4 offset : POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};


PixelInputType main(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 0.f;
	input.offset.w = 1.f;

	output.position = mul(input.offset, IViewRot);
	output.position = mul(output.position, IRot);
	output.position += input.position;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(output.position, WorldViewProj);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	//The color is sent through to the pixel shader here.

	// Store the particle color for the pixel shader. 
	output.color = input.color;

	return output;
}