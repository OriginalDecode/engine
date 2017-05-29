//---------------------------------
//	Deferred Ambient Pixel Shaders
//---------------------------------
cbuffer Ambient : register(b0)
{
	float4 camera_position;
	row_major float4x4 InvertedProjection;
	row_major float4x4 InvertedView;
	row_major float4x4 ShadowMVP;
	float4 light_direction;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D DepthTexture	 	: register ( t7 );
Texture2D ShadowTexture		: register ( t8 );
TextureCube CubeMap		 	: register ( t9 );
Texture2D SSAOTexture		: register ( t10 );

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

	float roughness = depth.y;
	float roughnessOffsetted = pow(8192, roughness);

	float4 substance = (0.04f - 0.04f * metalness) + albedo * metalness;
	float4 metalnessAlbedo = albedo - (albedo * metalness);

	float x = input.uv.x * 2.f - 1.f;
	float y = (1.f - input.uv.y) * 2.f - 1.f;
	float z = depth.x; 

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul (worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, InvertedView);	

	float4 ssao = SSAOTexture.Sample(point_Clamp, input.uv);
	
	float ao = 1.f;
	
	float3 toEye = normalize(camera_position.xyz - worldPosition.xyz);
	float3 reflection_fresnel = ReflectionFresnel(substance, normal, -toEye, 1 - roughnessOffsetted);
	float3 reflectionVector = reflect(toEye, normal.xyz);
	float3	ambientDiffuse = CubeMap.SampleLevel(point_Clamp, reflectionVector, 9).rgb * 
	(metalness * 0.2) + metalnessAlbedo * (1 - reflection_fresnel) * ao;
  
	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, 12);
    
	float3 ambientSpec = CubeMap.SampleLevel(point_Clamp, reflectionVector,lysMipMap).xyz * ao * reflection_fresnel;
	
	float3 final_color = saturate(ambientDiffuse + ambientSpec) ;
	float NdotL = dot(normal.xyz, -light_direction);
	
	/* Shadow */
	
	float4 newPos = worldPosition + (normal * 0.4);
	newPos.w = 1;
	float4 shadowVec = mul(newPos, ShadowMVP);
	shadowVec.xyz /= shadowVec.w;
	shadowVec.y = -shadowVec.y;
	shadowVec.x = shadowVec.x;
	shadowVec.xy += 1;
	shadowVec.xy *= 0.5;

	float compareValue = shadowVec.z;
	float sampleValue = ShadowTexture.Sample(point_Clamp, shadowVec.xy).x;
	if(sampleValue < compareValue)
 		final_color *= 0.42;

 	/* Shadow end */

 	float3 dir_color = float3(1,0.9,0.6);
	float3 output = saturate(final_color * dir_color * NdotL);
	
	return float4(output + (ambientDiffuse * 0.42), 1.f);
};
