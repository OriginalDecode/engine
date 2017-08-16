//---------------------------------
//	Samplers & Textures
//---------------------------------
struct DeferredPixelData
{
    float4 world_pos;
    float4 albedo;
    float4 substance;
    float4 normal;
    float4 metalnessAlbedo;
	float4 metalness;
    float4 depth;
    float roughness;
	float roughnessOffset;

};

const float k0 = 0.00098, k1 = 0.9921; 
const float specMax = (2 / (0.0014 * 0.0014)) - 2;

SamplerState sampler0 : register (ps, s0 );
Texture2D AlbedoTexture  : register (ps, t0 );
Texture2D NormalTexture  : register (ps, t1 );
Texture2D DepthTexture	 : register (ps, t7 );

DeferredPixelData CalculateDeferredPixelData(float2 tex_coord)
{
    DeferredPixelData data = (DeferredPixelData)0;
  
    data.albedo = AlbedoTexture.Sample(sampler0, tex_coord);
	data.normal = NormalTexture.Sample(sampler0, tex_coord);
	float4 depth = DepthTexture.Sample(sampler0, tex_coord);
    data.metalness = float4(data.normal.w, data.normal.w, data.normal.w, data.normal.w);
	data.normal *= 2.f;
	data.normal -= 1.f;
	data.depth = depth;
	data.roughness = depth.y;
	float ao = 1.0f;
	float4 substance = (0.04f - 0.04f * data.metalness) + data.albedo * data.metalness;
	data.substance = substance;
	data.metalnessAlbedo = data.albedo - (data.albedo * data.metalness);
	data.roughnessOffset = pow(8192, data.roughness);
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

LightVectors CalculateLightVectorsDirectional(DeferredPixelData pixel_data, float3 cam_pos, float3 light_dir)
{
    LightVectors vectors = (LightVectors)0;
    vectors.toEye = normalize(cam_pos - pixel_data.world_pos.xyz);	
	// vectors.toLight = light_pos - pixel_data.world_pos.xyz;
	vectors.light_dir = light_dir;//normalize(vectors.toLight);
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

float RoughToSPow(float fRoughness)
{
	return (2 / (fRoughness * fRoughness)) - 2;
};

float GetSpecPowToMip(float fSpecPow, int nMips)
{
	float fSmulMaxT = (exp2(-10 / sqrt(fSpecPow)) - k0) / k1;
	float fMaxT = exp2(-10 / sqrt(specMax) - k0) / k1;
	return float(nMips - 1) * (1 - clamp(fSmulMaxT / fMaxT, 0.0, 1.0));
};

float3 ReflectionFresnel(const float3 substance, const float3 light_dir, const float3 half_vector, float roughness)
{
	float LdotH = dot(light_dir, half_vector);
	LdotH = saturate(LdotH);
	LdotH = 1 - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1-substance);
	fresnel = fresnel / (6 - 5 * roughness);
	fresnel = substance + fresnel;
	return fresnel;
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