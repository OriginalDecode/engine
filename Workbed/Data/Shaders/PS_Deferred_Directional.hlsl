//---------------------------------
//	Deferred Lightmesh Pixel Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer DirectionalLight : register( b0 )
{
	row_major float4x4 InvertedProjection; 
	row_major float4x4 InvertedView; 
	float4 color;
	float4 cam_pos;
	row_major float4x4 shadowMVP;
    float4 direction;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

Texture2D ShadowTexture	 : register ( t3 );

//---------------------------------
//	Deferred Lightmesh Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Deferred Lightmesh Pixel Shader
//---------------------------------
#include "PBL_Functions.hlsl"
float4 PS(VS_OUTPUT input) : SV_Target
{

	DeferredPixelData data = CalculateDeferredPixelData(input.uv);
	//LightVectors vectors = CalculateLightVectors(data, camPosition, position);
	float3 light_pos = float3(256,256,256);
  	float3 toEye = normalize(cam_pos - data.world_pos.xyz);	
	float3 light_dir = normalize(-direction.xyz);
	float3 halfVec = normalize(light_dir + toEye);

	float NdotL = saturate(dot(data.normal, light_dir));
	float HdotN = saturate(dot(halfVec, data.normal));
	float NdotV = saturate(dot(data.normal, toEye));
	
	float3 F = saturate(Fresnel(data.substance, -light_dir, halfVec));
	float3 D = saturate(D_GGX(HdotN,(data.roughness + 1) / 2.f));
	float3 V = saturate(V_SchlickForGGX((data.roughness + 1) / 2.f, NdotV, NdotL));

    float3 color_ = float3(1,1,1);
	float3 directSpec = (F * D * V);
	float3 final_color = directSpec * NdotL * color;
	//float3 final_color = ( directSpec * ((attenuation * angularAttenuation) * color)) * 200;

	float4 newPos = data.world_pos + (data.normal * 0.4);
	newPos.w = 1;
	float4 shadowVec = mul(newPos, shadowMVP);
	shadowVec.xyz /= shadowVec.w;
	shadowVec.y = -shadowVec.y;
	shadowVec.x = shadowVec.x;
	shadowVec.xy += 1;
	shadowVec.xy *= 0.5;

	float compareValue = shadowVec.z;
	float sampleValue = ShadowTexture.Sample(point_Clamp, shadowVec.xy).x;
	if(sampleValue < compareValue)
 		final_color = 0;
	return float4(final_color, 1);
};