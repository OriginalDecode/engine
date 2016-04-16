matrix World;
matrix Projection;
matrix View;

Texture2D AlbedoTexture;

SamplerState sampleLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION0;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
}
