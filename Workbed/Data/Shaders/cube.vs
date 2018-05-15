cbuffer Matrices : register(b0)
{
	row_major matrix World;
	row_major matrix View;
	row_major matrix Projection;
};

struct VS_INPUT
{
	float4 Pos 		: POSITION;
	float4 Color 	: COLOR;
};

struct VS_OUTPUT
{
	float4 Pos 		: SV_POSITION;
	float4 Color	: COLOR;
};

VS_OUTPUT main(VS_INPUT input)

{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	return output;
}