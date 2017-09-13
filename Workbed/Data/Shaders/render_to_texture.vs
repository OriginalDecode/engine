struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = input.pos;
	output.uv = input.uv;
	return output;
};