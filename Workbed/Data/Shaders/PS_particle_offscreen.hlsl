//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

SamplerState sampler0 : register ( s0 );
Texture2D DepthTexture : register ( t8 );
//---------------------------------
//	Line3D Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
	float2 depth : DEPTH;
};

//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = input.depth.x;
	float depth0 = DepthTexture.Sample(sampler0, input.uv);
	return float4(depth, depth, depth, depth);
};
