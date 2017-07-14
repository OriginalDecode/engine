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
	float4x4 InvProjection;
	float4 texel_size;
};

//---------------------------------
//	Render to Texture Vertex Shader
//---------------------------------

float3 uvToEye(float2 texCoord, float depth){
	float x = texCoord.x * 2.0 - 1.0;
	float y = (1 - texCoord.y) * 2.0 - 1.0;
	float4 clipPos = float4(x , y, depth, 1.0f);
	float4 viewPos = mul(clipPos, InvProjection);
	return viewPos.xyz / viewPos.w;
}


float projectZ(float z, float near, float far) 
{
	return far*(z+near)/(z*(far-near));
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = DepthTexture.Sample(sampler0, input.uv).x;
	if(depth > 1)
		discard;

	float3 posEye = uvToEye(input.uv, depth);
	
	half3 ddx0 = DepthTexture.Sample(sampler0, input.uv + float2(texel_size.x, 0)).rgb - posEye;
	half3 ddx1 = posEye - DepthTexture.Sample(sampler0, input.uv + float2(0, texel_size.y)); // uvToEye(input.uv + float2(0, texel_size.y), depth);
	if(abs(ddx0.z) > abs(ddx1.z))
		ddx0 = ddx1;

	half3 ddy0 = DepthTexture.Sample(sampler0, input.uv + float2(-texel_size.x, 0)).rgb - posEye; //uvToEye(input.uv + float2(-texel_size.x, 0), depth) - posEye;
	half3 ddy1 = posEye - DepthTexture.Sample(sampler0, input.uv + float2(0, -texel_size.y)).rgb; //uvToEye(input.uv + float2(0, -texel_size.y), depth);

	if(abs(ddy0.z) > abs(ddy1.z))
		ddy0 = ddy1;

	float3 n = normalize(cross(ddx0, ddy0));

	

	return float4(n, 1);
};