cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
};

struct VS_INPUT
{
	float4 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 worldpos : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.worldpos = mul(input.pos, World);
	return output;
};