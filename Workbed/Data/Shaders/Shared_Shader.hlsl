#pragma once
float V_SchlickForGGX(float aRoughness, float aNdotV, float aNdotL)
{
	float k = aRoughness * aRoughness * 0.5f;
	float G1V = aNdotV * (1.f - k) + k;
	float G1L = aNdotL * (1.f - k) + k;

	return 0.25f / (G1V * G1L);
}

float D_GGX(float aHdotN, float aRoughness)
{
	float m = aRoughness*aRoughness;
	float m2 = m*m;
	float Denominator = aHdotN*aHdotN*(m2 - 1.f) + 1.f;
	float D = m2 / (3.14159*Denominator*Denominator);
	return D;
}

float3 Fresnel(const float3 aSubstance, const float3 aLightDir, const float3 aHalfVec)
{
	float LdotH = dot(aLightDir, aHalfVec);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1.f - aSubstance);
	fresnel = aSubstance + fresnel;
	return fresnel;
}

float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return attenuation;
}

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 0.9f - (someDistance / (someRange + 0.00001f));
	return fallOff;
}

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return totalAttenuation;
}