//---------------------------------
//	Deferred Ambient Pixel Shaders
//---------------------------------
cbuffer CameraPosition : register(b0)
{
	float4 camera_position;
	row_major float4x4 InvertedProjection;
	row_major float4x4 InvertedView;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D DepthTexture	 	: register ( t2 );
TextureCube CubeMap		 	: register ( t3 );

//---------------------------------
//	Deferred Ambient Pixel Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION0;
	float2 uv	: TEXCOORD;
};

//---------------------------------
//	Deferred Ambient Pixel Shader
//---------------------------------
const float k0 = 0.00098,
			k1 = 0.9921; 
const float specMax = (2 / (0.0014 * 0.0014)) - 2;
float GetSpecPowToMip(float fSpecPow, int nMips)
{
	float fSmulMaxT = (exp2(-10 / sqrt(fSpecPow)) - k0) / k1;
	float fMaxT = exp2(-10 / sqrt(specMax) - k0) / k1;
	return float(nMips - 1) * (1 - clamp(fSmulMaxT / fMaxT, 0.0, 1.0));
}

float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return attenuation;
}

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 1.f - (someDistance / (someRange + 0.001));
	return fallOff;
}

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return totalAttenuation;
}

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
}

float RoughToSPow(float fRoughness)
{
	return (2 / (fRoughness * fRoughness)) - 2;
}

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
	float4 depth = DepthTexture.Sample(point_Clamp, input.uv);
	if(depth.x <= 0.f)
		discard;

	float4 albedo = AlbedoTexture.Sample(point_Clamp, input.uv);	
	float4 normal = NormalTexture.Sample(point_Clamp, input.uv);
	
	float4 metalness = float4(normal.w, normal.w, normal.w, normal.w);

	normal *= 2;
	normal -= 1;

	float roughness = 1;//depth.y;
	
	float roughnessOffsetted = pow(8192, roughness);

	float4 substance = (0.04f - 0.04f * metalness) + albedo * metalness;
	float4 metalnessAlbedo = albedo - (albedo * metalness);

	float x = input.uv.x * 2.f - 1.f;
	float y = (1.f - input.uv.y) * 2.f - 1.f;
	float z = depth.x; 	
	float3 ao = float3(1,1,1);
	
	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul (worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, InvertedView);	

	float3 toEye = normalize(worldPosition.xyz - camera_position.xyz);
	float3 reflection_fresnel = ReflectionFresnel(substance, normal, -toEye, 1 - roughnessOffsetted);
	float3 reflectionVector = reflect(toEye, normal.xyz);
	float3	ambientDiffuse = CubeMap.SampleLevel(point_Clamp, reflectionVector, 9).rgb * 
	(metalness * 0.2) + albedo * (1 - reflection_fresnel) * ao;
  
	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, 12);
    
	float3 ambientSpec = CubeMap.SampleLevel(point_Clamp, reflectionVector,lysMipMap).xyz * ao * reflection_fresnel;
	
	float3 finalColor = (ambientDiffuse + ambientSpec);

	// float3 position = float3(95,2,32);

	// float3 toLight = position - worldPosition.xyz;
	// float3 lightDir = normalize(toLight);
	// float3 halfVec = normalize(lightDir + toEye);

	// float NdotL = dot(normal, lightDir);
	// float HdotN = saturate(dot(halfVec, normal));
	// float NdotV = saturate(dot(normal, -toEye));

	// float3 F = float3(1,1,1);
	// float D = 1;
	// float V = 1;
	// F += saturate(Fresnel(substance, -lightDir, halfVec));
	// D += saturate(D_GGX(HdotN,(roughness + 1.f) / 2.f));
	// V += saturate(V_SchlickForGGX((roughness + 1.f) / 2.f, NdotV, NdotL));

 	// float3 lightToPixel = normalize(-toLight);
 	// float spotFactor = dot(lightToPixel, normalize(float3(1,0,0)));

	//  if(spotFactor < 0)
	//  	spotFactor = 0;

	// float angularAttenuation =  1 - (1 - spotFactor) * 1 / (1 - cos(6)) / 2;

	// float ln = length(toLight);
	// float attenuation = CalculateTotalAttenuation(ln, 10);
	// float attNdotL =  NdotL * attenuation * angularAttenuation;

	// float3 directSpec = D * F * V * attenuation * angularAttenuation * float4(1,0,0,1);

	float3 output = (finalColor * 0.5);
	
	return float4(output, 1.f);
};
