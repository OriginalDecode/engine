//---------------------------------
//	Deferred Lightmesh Pixel Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------


cbuffer Pointlight : register(b0)
{
	row_major float4x4 InvertedProjection; 
	row_major float4x4 InvertedView; 
	float4 color;
	float4 position;
	float4 camPosition;
	row_major float4x4 shadowMVP;

};
//---------------------------------
//	Samplers & Textures
//---------------------------------

Texture2D ShadowTexture  : register ( t3 );
//---------------------------------
//	Deferred Lightmesh Pixel Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float4 uv	: POSITION;
	float4 range : RANGE;
};

//---------------------------------
//	Deferred Lightmesh Pixel Shaderf
//---------------------------------
#include "PBL_Functions.hlsl"
float4 PS(VS_OUTPUT input) : SV_Target
{
	input.uv /= input.uv.w;
	float2 texCoord = input.uv.xy;

	DeferredPixelData data = CalculateDeferredPixelData(texCoord);
	LightVectors vectors = CalculateLightVectors(data, camPosition, position);
	float4 substance_replace = float4(1,1,1,1);
	float3 F = saturate(Fresnel(data.substance, -vectors.light_dir, vectors.halfVec));
	float D = saturate(D_GGX(vectors.HdotN,(data.roughness ) / 2.f));
	float V = saturate(V_SchlickForGGX((data.roughness  ) / 2.f, vectors.NdotV, vectors.NdotL));


	float ln = length(vectors.toLight);
	float attenuation = CalculateTotalAttenuation(ln, input.range.x );
	float3 directSpec = float3(1,1,1) + (F * D * V);
	float intensity = 100;
	float3 final_color = saturate(directSpec * color * attenuation) * intensity;

	return float4(final_color, 1);
};