//---------------------------------
//	Samplers & Textures
//---------------------------------
struct DeferredPixelData
{
    float4 world_pos;
    float4 albedo;
    float4 substance;
    float4 normal;
    float roughness;
};

SamplerState point_Clamp : register (ps, s0 );
Texture2D AlbedoTexture  : register (ps, t0 );
Texture2D NormalTexture  : register (ps, t1 );
Texture2D DepthTexture	 : register (ps, t2 );

DeferredPixelData CalculateDeferredPixelData(float2 tex_coord)
{
    DeferredPixelData data = (DeferredPixelData)0;
  
    data.albedo = AlbedoTexture.Sample(point_Clamp, tex_coord);
	data.normal = NormalTexture.Sample(point_Clamp, tex_coord) * 2 - 1;
	float4 depth = DepthTexture.Sample(point_Clamp, tex_coord);
    float4 metalness = float4(data.normal.w, data.normal.w, data.normal.w, data.normal.w);
	data.roughness = depth.y;
	float ao = 1.0f;
	float4 substance = (0.04f - 0.04f * metalness) + data.albedo * metalness;
	data.substance = substance;
    float x = tex_coord.x * 2.f - 1.f;
	float y = (1.f - tex_coord.y) * 2 - 1;
	float z = depth.x;

	data.world_pos = float4(x, y, z, 1.f);
	
	data.world_pos = mul(data.world_pos, InvertedProjection);
	data.world_pos = data.world_pos / data.world_pos.w;
	data.world_pos = mul(data.world_pos, InvertedView);

    return data;
}

struct LightVectors
{
    float3 toEye;
    float3 toLight;
    float3 light_dir;
    float3 halfVec;

    float NdotL;
    float HdotN;
    float NdotV;
};

LightVectors CalculateLightVectors(DeferredPixelData pixel_data, float3 cam_pos, float3 light_pos)
{
    LightVectors vectors = (LightVectors)0;
    vectors.toEye = normalize(cam_pos - pixel_data.world_pos.xyz);	
	vectors.toLight = light_pos - pixel_data.world_pos.xyz;
	vectors.light_dir = normalize(vectors.toLight);
	vectors.halfVec = normalize(vectors.light_dir + vectors.toEye);

	vectors.NdotL = dot(pixel_data.normal, vectors.light_dir);
	vectors.HdotN = saturate(dot(vectors.halfVec, pixel_data.normal));
	vectors.NdotV = saturate(dot(pixel_data.normal, vectors.toEye));
    
    return vectors;
};


float V_SchlickForGGX(float aRoughness, float aNdotV, float aNdotL)
{
	float k = (aRoughness * aRoughness) * 0.5f;
	float G1V = aNdotV * (1.f - k) + k;
	float G1L = aNdotL * (1.f - k) + k;

	return 0.25f / (G1V * G1L);
};

float D_GGX(float aHdotN, float aRoughness)
{
	float m = aRoughness*aRoughness;
	float m2 = m*m;
	float Denominator = aHdotN*aHdotN*(m2 - 1.f) + 1.f;
	float D = m2 / (3.14159*Denominator*Denominator);
	return D;
};

float3 Fresnel(const float3 aSubstance, const float3 aLightDir, const float3 aHalfVec)
{
	float LdotH = dot(aLightDir, aHalfVec);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1.f - aSubstance);
	fresnel = aSubstance + fresnel;
	return fresnel;
};

float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return attenuation;
};

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 0.9f - (someDistance / (someRange + 0.00001f));
	return fallOff;
};

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return totalAttenuation;
};