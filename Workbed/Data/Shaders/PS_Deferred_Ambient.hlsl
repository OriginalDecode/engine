//---------------------------------
//	Deferred Ambient Pixel Shaders
//---------------------------------
cbuffer CameraPosition : register(b0)
{
	float4 camera_position;
	row_major float4x4 InvertedProjection;
	row_major float4x4 InvertedView;
	row_major float4x4 shadowMVP;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D DepthTexture	 	: register ( t2 );
Texture2D ShadowTexture		: register ( t3 );
TextureCube CubeMap		 	: register ( t4 );

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
	(metalness * 0) + albedo * (1 - reflection_fresnel) * ao;
  
	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, 12);
    
	float3 ambientSpec = CubeMap.SampleLevel(point_Clamp, reflectionVector,lysMipMap).xyz * ao * reflection_fresnel;
	
	float3 finalColor = (ambientDiffuse + ambientSpec);

	float4 shadowVec = mul(worldPosition, shadowMVP);
	shadowVec.xyz /= shadowVec.w;
	shadowVec.y = -shadowVec.y;
	shadowVec.x = shadowVec.x;
	shadowVec.xy += 1;
	shadowVec.xy *= 0.5;
	float compareValue = shadowVec.z;


	float sampleValue = ShadowTexture.Sample(point_Clamp, shadowVec.xy).x;
	if(sampleValue < compareValue - 0.005 )
	{
		if(shadowVec.x > 0 && shadowVec.x < 1 && shadowVec.y > 0 && shadowVec.y < 1 && shadowVec.z < 1 && shadowVec.z > 0)
		{
			finalColor *= 0.42;
		}
	}

	return float4(finalColor, 1.f);
};
