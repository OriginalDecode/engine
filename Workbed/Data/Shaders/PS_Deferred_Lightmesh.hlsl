//---------------------------------
//	Deferred Lightmesh Pixel Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Pointlight : register(b0)
{
	row_major float4x4 InvertedProjection; 
	row_major float4x4 View; 
	float4 color;
	float4 position;
	float4 camPosition;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp : register ( s0 );
Texture2D AlbedoTexture  : register ( t0 );
Texture2D NormalTexture  : register ( t1 );
Texture2D DepthTexture	 : register ( t2 );

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
	float totalAttenuation = saturate(CalculateAttenuation(someDistance) );
	return totalAttenuation;
}

float Attenuation(float3 aLightVec, float aRange)
{
	float distance = length(aLightVec);
	//return 1 - (distance / aRange);

	float attenuation = 1.f;// / (1.f + 0.1f * distance + 0.01f * distance * distance);
	float fallOff = 0.9f - (distance / (aRange + 0.00001f));
	return saturate(attenuation * fallOff);
}

//---------------------------------
//	Deferred Lightmesh Pixel Shader
//---------------------------------

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



float4 PS(VS_OUTPUT input) : SV_Target
{
	input.uv /= input.uv.w;
	float2 texCoord = input.uv.xy;

	float4 albedo = AlbedoTexture.Sample(point_Clamp, texCoord);
	float4 normal = NormalTexture.Sample(point_Clamp, texCoord);
	float4 depth = DepthTexture.Sample(point_Clamp, texCoord);
	
	// float4 metalness = float4(normal.w, normal.w, normal.w, normal.w);
	// float roughness = depth.y;
	// float roughnessOffsetted = pow(8192, roughness);
	// float ao = 1.0f;
	// float4 substance = (0.04f - 0.04f * metalness) + albedo * metalness;
	  
	float x = texCoord.x * 2.f - 1.f;
	float y = (1.f - texCoord.y) * 2.f - 1.f;
	float z = depth.x;
	
	float4 worldPosition = float4(x, y, z, 1.f);
	
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, View);

	float3 viewPos = camPosition.xyz;
	float3 toEye = normalize(worldPosition.xyz - viewPos);	
	float3 toLight = worldPosition - position;
	float3 lightDir = normalize(toLight);
	//float3 halfVec = normalize(lightDir + toEye);
	float lambert = dot(lightDir, float3(normal.xyz));

	normal = normalize(normal);
	float3 m_NdotL = dot(normal, lightDir);
	return float4(m_NdotL.rgb, 1.f);

	// float3 nnormal = normalize(normal);
	// float3 NdotL = dot(nnormal,lightDir);//saturate(dot(normal, lightDir));
	// float HdotN = saturate(dot(halfVec, nnormal));
	// float NdotV = saturate(dot(nnormal, toEye));
	// float3 F = Fresnel(substance, lightDir, halfVec);
	// float D = saturate(D_GGX(HdotN,(roughness + 1.f) / 2.f));
	// float V = saturate(V_SchlickForGGX((roughness + 1.f) / 2.f, NdotV, NdotL));
	// //float lambert = NdotL;
	// float distance = length(toLight);
	// float attenuation = Attenuation(toLight, input.range);
	// float intensity = color.w;
	// float3 lightColor = color.rgb * 10 * attenuation;
	
	// float3 directSpec = NdotL;
	//return float4( directSpec.xyz, 1.f);
};