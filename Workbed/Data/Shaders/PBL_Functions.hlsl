#define SATURATE(_value) saturate(_value) 

const float k0 = 0.00098, 
			k1 = 0.9921; 
const float specMax = (2 / (0.0014 * 0.0014)) - 2;
static const float PI = 3.14159265;

SamplerState sampler0 	: register( ps, s0 );
SamplerState sampler1 	: register( ps, s1 );
Texture2D AlbedoTexture : register( ps, t0 );
Texture2D NormalTexture : register( ps, t1 );
Texture2D DepthTexture	: register( ps, t7 );
TextureCube CubeMap		: register( ps, t9 );

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
	LdotH = normalize(LdotH);
	LdotH = 1 - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1-substance);
	fresnel = fresnel / (6 - (5 * roughness));
	fresnel = substance + fresnel;
	return fresnel;
};

float4 Fresnel(const float4 aSubstance, const float4 aLightDir, const float4 aHalfVec)
{
	float LdotH = dot(aLightDir, aHalfVec);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float4 fresnel = LdotH * (1.f - aSubstance);
	fresnel = aSubstance + fresnel;
	return fresnel;
};

float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return SATURATE(attenuation);
};

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 0.9f - (someDistance / (someRange + 0.00001f));
	return SATURATE(fallOff);
};

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return SATURATE(totalAttenuation);
};
