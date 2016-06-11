struct VS_INPUT_POS_NORMAL_UV_BINORMAL_TANG
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
};

struct PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
	float3 WorldPos : POSITION;
};


PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG VS(VS_INPUT_POS_NORMAL_UV_BINORMAL_TANG input)
{
	PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG output = (PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	
	output.Tang = mul(input.Tang, World);
	output.WorldPos = mul(input.Pos, World);

	return output;
}