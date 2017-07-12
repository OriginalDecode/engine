//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

SamplerState point_sample : register ( s0 );
Texture2D ParticleTexture : register ( t0 );
Texture2D NormalTexture : register ( t1 );
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

struct PBuffer
{
	float4 Diffuse;
	float4 Normal;
};

PBuffer PS(VS_OUTPUT input) : SV_Target
{
	float4 diff = ParticleTexture.Sample(point_sample, input.uv);
	float4 normal = NormalTexture.Sample(point_sample, input.uv);

	PBuffer output = (PBuffer)0;

	output.Diffuse = diff;
	output.Normal = float4(normal.rgb, diff.a);
	return output;
};
