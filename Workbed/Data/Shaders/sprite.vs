cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	float2 Position;
	float2 Scale;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, World);
	
	output.pos.x *= 1;
	output.pos.y *= 1;
	
	output.pos = mul(output.pos, View);
	
	output.pos.x += Position.x;
	output.pos.y += Position.y; 	
	
	output.pos = mul(output.pos, Projection);
	output.uv = input.uv;
	
	return output;
};