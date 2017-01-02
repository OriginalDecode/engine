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
    float4 direction;

};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp : register ( s0 );
Texture2D AlbedoTexture  : register ( t0 );
Texture2D NormalTexture  : register ( t1 );
Texture2D DepthTexture	 : register ( t2 );
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
//	Deferred Lightmesh Functions
//---------------------------------
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

//---------------------------------
//	Deferred Lightmesh Pixel Shader
//---------------------------------

float V_SchlickForGGX(float aRoughness, float aNdotV, float aNdotL)
{
	float k = (aRoughness * aRoughness) * 0.5f;
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

float4 PS(VS_OUTPUT input) : SV_Target
{
		input.uv /= input.uv.w;
	float2 texCoord = input.uv.xy;

	float4 albedo = AlbedoTexture.Sample(point_Clamp, texCoord);
	float4 normal = NormalTexture.Sample(point_Clamp, texCoord) * 2 - 1;
	float4 depth = DepthTexture.Sample(point_Clamp, texCoord);
	
	float4 metalness = float4(normal.w, normal.w, normal.w, normal.w);
	float roughness = depth.y;
	float roughnessOffsetted = pow(8192, roughness);
	float ao = 1.0f;
	float4 substance = (0.04f - 0.04f * metalness) + albedo * metalness;

	float x = texCoord.x * 2.f - 1.f;
	float y = (1.f - texCoord.y) * 2 - 1;
	float z = depth.x;

	float4 worldPosition = float4(x, y, z, 1.f);
	
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, InvertedView);
	
	float3 toEye = normalize(camPosition.xyz - worldPosition.xyz);	
	float3 toLight = position - worldPosition.xyz;
	float3 lightDir = normalize(toLight);
	float3 halfVec = normalize(lightDir + toEye);

	float NdotL = dot(normal, lightDir);
	float HdotN = saturate(dot(halfVec, normal));
	float NdotV = saturate(dot(normal, -toEye));

	float3 F = float3(1,1,1);
	float D = 1;
	float V = 1;
	F += saturate(Fresnel(substance, -lightDir, halfVec));
	D += saturate(D_GGX(HdotN,(roughness + 1.f) / 2.f));
	V += saturate(V_SchlickForGGX((roughness + 1.f) / 2.f, NdotV, NdotL));

    float3 lightToPixel = normalize(worldPosition.xyz - position.xyz);
    float spotFactor = dot(lightToPixel, normalize(direction));
	if(spotFactor < 0)
		spotFactor = 0;
	float angularAttenuation =  (1 - (1 - spotFactor) * 1 / (1 - input.cosAngle.x)) / 2;

	float ln = length(toLight);
	float attenuation = CalculateTotalAttenuation(ln, 2); // current problem
		
	float attNdotL =  NdotL * attenuation * angularAttenuation;

	float3 directSpec = D * F * V * attenuation * angularAttenuation * color ;

	return float4(directSpec, 1);
};
//	float angularAttenuation =  (1 - (1 - spotFactor) * 1 / (1 - input.cosAngle.x)) / 2;

    // float spotFactor = dot(lightToPixel, normalize(direction));

		//Shadow
	// float4 newPos = worldPosition + (normal * 0.4);
	// newPos.w = 1;
	// float4 shadowVec = mul(newPos, shadowMVP);
	// shadowVec.xyz /= shadowVec.w;
	// shadowVec.y = -shadowVec.y;
	// shadowVec.x = shadowVec.x;
	// shadowVec.xy += 1;
	// shadowVec.xy *= 0.5;

	// float compareValue = shadowVec.z;

	// float sampleValue = ShadowTexture.Sample(point_Clamp, shadowVec.xy).x;
	// if(sampleValue < compareValue)
	// {
	// 	final_color = 0;
	// }