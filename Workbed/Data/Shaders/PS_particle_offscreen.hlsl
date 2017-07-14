//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

SamplerState sampler0 : register ( s0 );
Texture2D DepthTexture : register ( t7 );
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
	float4 eyePos : POSITION0;
	float4x4 projection : MATRIX0;
	
};
cbuffer Matrices : register(b0)
{
	row_major float4x4 Projection;
};
//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float r = 2;
	float3 normal = float3(input.uv.xy * 2 - 1, 1);
	float r2 = dot(normal.xy, normal.xy);
	if(r2 > 1.0)
		discard;

	normal.z = -sqrt(1.0 - r2);	

	float4 pixel = float4(input.eyePos + normal * r, 1.0);
	float4 clip = mul(pixel, Projection);
	float depth = clip.z / clip.w;

	float diffuse = max(0.0, dot(normal, float3(0,1,0)));
	return float4(depth,depth,depth,depth);
};
