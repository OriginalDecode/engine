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

Texture2D ParticleTexture : register ( t5 );
Texture2D ShadowTexture  : register ( t8 );

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
	float3 F = saturate(Fresnel(data.substance, -vectors.light_dir, vectors.halfVec));
	float D = saturate(D_GGX(vectors.HdotN,(data.roughness + 1 ) / 2.f));
	float V = saturate(V_SchlickForGGX((data.roughness + 1 ) / 2.f, vectors.NdotV, vectors.NdotL));

	float4 particle = ParticleTexture.Sample(point_Clamp, texCoord);


	float ln = length(vectors.toLight);
	float attenuation = CalculateTotalAttenuation(ln, input.range.x );
	float3 directSpec =  (F * D * V);
	float intensity = 100;
	float3 final_color = saturate(directSpec * color * attenuation) * intensity;

	float4 particle_out = saturate(particle * color );
	return float4(final_color, 1);
};