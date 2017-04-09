#include "AtmosphereDefinitions.hlsl"

cbuffer DefaultBuffer : register( b0 )
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;

};

cbuffer AtmosphereBuffer : register( b1 )
{
	float3 sky_spec_rad_to_lum;
	float3 sun_spec_rad_to_lum;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
};

struct VS_OUTPUT
{


};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);

	output.uv = input.uv;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	output.worldpos = input.pos;


	return output;
};

void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{

};


float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).r;


	return float4(1,1,1,1);
};