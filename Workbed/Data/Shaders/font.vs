cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	float4 scale; //bad variable, junk data for font, move pos & scale to it?
	float2 Position;
	float2 Scale;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float4 uv : TEXCOORD;
	
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos.x += Position.x;
	output.pos.y += Position.y; 	
	output.pos = mul(output.pos, Projection);
	output.color = input.color;	
	output.uv = input.uv;
	
	return output;
};