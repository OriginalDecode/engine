//---------------------------------
//	Deferred Lightmesh Pixel Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Spotlight : register( b0 )
{
	row_major float4x4 InvertedProjection; 
	row_major float4x4 InvertedView; 
	float4 color;
	float4 position;
	float4 camPosition;
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
	float4 uv	: POSITION;
	float2 cosAngle : COSANGLE;
    float2 range : RANGE;
};

//---------------------------------
//	Deferred Lightmesh Pixel Shader
//---------------------------------
#include "PBL_Functions.hlsl"
float4 PS(VS_OUTPUT input) : SV_Target
{
	input.uv /= input.uv.w;
	float2 texCoord = input.uv.xy;

	DeferredPixelData data = CalculateDeferredPixelData(texCoord);
	LightVectors vectors = CalculateLightVectors(data, camPosition, position);
	
	float3 F = 1 + saturate(Fresnel(data.substance, -vectors.light_dir, vectors.halfVec));
	float3 D = 1 + saturate(D_GGX(vectors.HdotN,(data.roughness + 1) / 2.f));
	float3 V = 1 + saturate(V_SchlickForGGX((data.roughness + 1) / 2.f, vectors.NdotV, vectors.NdotL));

    float3 lightToPixel = normalize(-vectors.toLight);
    float spotFactor = max(0, dot(lightToPixel, normalize(direction)));

	float angularAttenuation = (1 - (1 - spotFactor) * ( 1 / ( 1 - input.cosAngle.x))) / 2;

	float ln = length(vectors.toLight);
	float attenuation = max(0, CalculateTotalAttenuation(ln, input.range.x));


	float3 directSpec = (D * F * V) * attenuation * angularAttenuation * color * 35;

	float3 final_color = directSpec;

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