//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

SamplerState point_sample : register ( s0 );
Texture2D ParticleTexture : register ( t0 );
//---------------------------------
//	Line3D Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = ParticleTexture.Sample(point_sample, input.uv);
	color.a = input.alpha.x;
	return color;
};
