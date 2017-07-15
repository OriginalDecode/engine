//---------------------------------
//	Render to Texture Vertex Shader
//---------------------------------
//---------------------------------
//	Render to Texture Vertex Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

SamplerState sampler0 : register ( s0 );
Texture2D DepthTexture : register ( t7 );

cbuffer CalcSSNormal
{
	float4x4 Projection;
	float4x4 InvProjection;
	float4 texel_size;
};

//---------------------------------
//	Render to Texture Vertex Shader
//---------------------------------

float3 uvToEye(float2 texCoord, float depth)
{
	float x = texCoord.x * 2.0 - 1.0;
	float y = (1 - texCoord.y) * 2.0 - 1.0;
	float4 clipPos = float4(x , y, depth, 1.0f);
	float4 viewPos = mul(clipPos, InvProjection);
	return viewPos.xyz / viewPos.w;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = DepthTexture.Sample(sampler0, input.uv).x;
	float maxDepth = 0.999;
	if(depth > maxDepth)
		discard;

	float3 posEye = uvToEye(input.uv, depth);


	float3 dx0 = DepthTexture.Sample(sampler0, input.uv + float2(texel_size.x, 0)).rgb;
	float3 dx1 = DepthTexture.Sample(sampler0, input.uv + float2(-texel_size.x, 0)).rgb;
	

	half3 ddx0 = uvToEye(input.uv + float2(texel_size.x, 0), dx0) - posEye;
	half3 ddx1 = posEye - uvToEye(input.uv + float2(-texel_size.x, 0), dx1);
	if(abs(ddx0.z) > abs(ddx1.z))
		ddx0 = ddx1;


	float3 dy0 = DepthTexture.Sample(sampler0, input.uv + float2(0, texel_size.y)).rgb;
	float3 dy1 = DepthTexture.Sample(sampler0, input.uv + float2(0, -texel_size.y)).rgb;

	half3 ddy0 = uvToEye(input.uv + float2(0, texel_size.y), dy0) - posEye; 
	half3 ddy1 = posEye - uvToEye(input.uv + float2(0, -texel_size.y), dy1);

	if(abs(ddy0.z) > abs(ddy1.z))
		ddy0 = ddy1;


	float3 n = normalize(cross(ddx0, ddy0));


	float4 normal = float4(n, depth);

	

	return normal;
};