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

struct GBuffer
{
	float4 Diffuse;
	float4 Normal;
	float4 Depth;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 diffuse = ParticleTexture.Sample(point_sample, input.uv);
	float4 normal = NormalTexture.Sample(point_sample, input.uv);
	float4 light_dir = float4(0,1,0,1);
	float4 color = float4(1,0,0,1);

	return diffuse;
};
