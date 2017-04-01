#include "S_variables_shader.fx"

static const float shLightModifier = 1.5f; //Multiper för det bakade ambienta ljuset
static const float cubeMapAmbientLight = 1.f; //Modifies how much of the cubemap is used in the ambient-diffuse-light //don't use
static const float cubeMapAmbientSpec = 0.25f; //Modifies how much of the cubemap is used in the ambient-specular-light
static const float emissiveMultiplier = 1.f; //Modifier to increase/decrease the emissive color
static const float decalEmissiveMultiplier = 3.f; //Modifier to increase/decrease the emissive color
static const float shadowEpsilon =  0.00075f; //don't use
static const float minimumAmbientLight = 0.0f; //The darkest color the ambient can have in each channel //don't use

static const float random_size = 256.f; //Size of the random-texture used for SSAO

static const float SSAO_Sample_Size = 0.1f; //Size of the SSAO
static const float SSAO_Intensity = 0.75f; //The intensity of the SSAO
static const float SSAO_Scale = 0.1f;
static const float SSAO_Bias = 0.15f;




static const float4 warm = float4(36/255.f, 34/255.f, 28/255.f, 1.f) * 1.1f;
static const float4 cold = float4(28/255.f, 33/255.f, 36/255.f, 1.f) * 1.1f;

static const float4 xPositiveColor = warm; //float4(1.f, 0.f, 0.f, 1.f);
static const float4 xNegativeColor = cold; //float4(0.f, 1.f, 0.f, 1.f);

static const float4 yPositiveColor = (warm + cold) / 2.f;//float4(1.f, 0.f, 1.f, 1.f);
static const float4 yNegativeColor = (warm + cold) / 2.f;//float4(0.f, 1.f, 1.f, 1.f);

static const float4 zPositiveColor = (warm + cold) / 2.f;//float4(0.f, 0.f, 1.f, 1.f);
static const float4 zNegativeColor = (warm + cold) / 2.f;//float4(1.f, 1.f, 0.f, 1.f);

/*
static float value = 0.2f;
static const float4 xPositiveColor = float4(value, 0.f, 0.f, value);
static const float4 xNegativeColor = float4(0.f, value, 0.f, value);

static const float4 yPositiveColor = float4(value, 0.f, value, value);
static const float4 yNegativeColor = float4(0.f, value, value, value);

static const float4 zPositiveColor = float4(0.f, 0.f, value, value);
static const float4 zNegativeColor = float4(value, value, 0.f, value);
*/






float2 g_screen_size = float2(1920, 1080);

//Variabels from Lys
static const float k0 = 0.00098f;
static const float k1 = 0.9921f;
static const float fakeLysMaxSpecularPower = (2.f / (0.0014f * 0.0014f)) - 2.f;
static const float fMaxT = (exp2(-10.f / sqrt((2.f / (0.0014f * 0.0014f)) - 2.f)) - 0.00098f) / 0.9921f;

float3 cameraPosition;

float TotalTime;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState linearSamplingClamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState pointSample
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState linearSample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState pointSampleWrap
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct DeferredPixelData
{
	float4 myAlbedo;
	float4 myNormal;
	float4 myDepth;
	float2 myTexCoord;
	float4 myWorldPosition;

	float4 myMetalness;
	float4 myMetalnessAlbedo;
	float4 mySubstance;
	float myAmbientocclusion;
	float myRoughness;
	float myRoughnessOffsetted;
	float4 myEmissive;
};

DeferredPixelData CalculateDeferredPixelData(float2 aTex)
{
	float4 albedoSample = AlbedoTexture.Sample(pointSample, aTex);
	float4 normalSample = NormalTexture.Sample(pointSample, aTex);
	float4 emissiveSample = EmissiveTexture.Sample(pointSample, aTex);
	float2 depthSample = DepthTexture.Sample(pointSample, aTex).xy;


	float4 albedoColor = float4(albedoSample.xyz, 1.f);
	

	float4 normal = float4(normalSample.xyz, 1.f);
	normal.xyz *= 2.0f;
	float normLength = length(normal.xyz);
	if(normLength != 0)
	{
		normal.xyz -= 1.f;
	}

	float metalness = normalSample.w;
	float roughness = depthSample.y;
	float depth = depthSample.x;

	float x = aTex.x * 2.f - 1.f;
	float y = (1.f - aTex.y) * 2.f - 1.f;
	float z = depth;

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, NotInvertedView);

	DeferredPixelData data = (DeferredPixelData)0;
	
	data.myAlbedo = albedoColor;
	data.myNormal = normal;
	data.myDepth = float4(depth, depth, depth, 1);
	data.myTexCoord = aTex;
	data.myWorldPosition = worldPosition;

	data.myMetalness = float4(metalness, metalness, metalness, metalness);
	data.myMetalnessAlbedo = data.myAlbedo - (data.myAlbedo * data.myMetalness);
	data.mySubstance = (0.04f - 0.04f * data.myMetalness) + data.myAlbedo * data.myMetalness;
	data.myRoughness = roughness;
	data.myRoughnessOffsetted = pow(8192, data.myRoughness);
	data.myAmbientocclusion = emissiveSample.w;
	data.myEmissive = float4(emissiveSample.xyz, 1.f);
	return data;
}

GBuffer CalculateGBuffer(float4 aPosition, float2 aUV, float3 aNormal, float3 aBiNormal, float3 aTangent)
{
	float3 norm = NormalTexture.Sample(linearSampling, aUV) * 2 - 1;
	
	aNormal = normalize(aNormal);
	aBiNormal = normalize(aBiNormal);
	aTangent  = normalize(aTangent);

	float3x3 tangentSpaceMatrix = float3x3(aTangent, aBiNormal, aNormal);
	norm = normalize(mul(norm, tangentSpaceMatrix));
	norm.xyz += 1.f;
	norm.xyz *= 0.5f;

	float depth = aPosition.z;

	GBuffer output;
	output.Albedo = AlbedoTexture.Sample(linearSampling, aUV);
	output.Albedo.x += minimumAmbientLight;
	output.Albedo.y += minimumAmbientLight;
	output.Albedo.z += minimumAmbientLight;

	output.Normal = float4(norm, 0.f);
	output.Normal.w = MetalnessTexture.Sample(linearSampling, aUV).x;
		
	output.Emissive = EmissiveTexture.Sample(linearSampling, aUV);
	output.Emissive *= emissiveMultiplier;
	output.Emissive.w = AOTexture.Sample(linearSampling, aUV).x;
	//output.Emissive.w = 1.f;

	output.Depth.x = depth;
	output.Depth.y = RoughnessTexture.Sample(linearSampling, aUV).x;
	return output;
}

struct PBLVertexShaderData
{
	float4 Position;
	float3 Normal;
	float2 Tex;
	float3 BiNormal;
	float3 Tangent;
	float4 WorldPosition;
};


PBLVertexShaderData CalcVertexShaderData(float4 aPosition, float3 aNormal
, float2 aTex, float3 aBiNormal, float3 aTangent)
{
	PBLVertexShaderData output = (PBLVertexShaderData)0;
	aPosition.xyz *= Scale;
	aPosition.w = 1.0f;

	Matrix mat = mul(World, ViewProjection);
	output.Position = mul(aPosition, mat);
	output.Tex = aTex;

	output.Normal = mul(float4(aNormal, 0), World).xyz;
	output.BiNormal = aBiNormal;
	output.Tangent = mul(float4(aTangent, 0), World).xyz;
	output.WorldPosition = mul(aPosition, World);

	return output;
}

PBLVertexShaderData CalcVertexShaderData_Instanced(float4 aPosition, float3 aNormal
, float2 aTex, float3 aBiNormal, float3 aTangent, Matrix aWorldMatrix, float3 aScale)
{
	PBLVertexShaderData output = (PBLVertexShaderData)0;
	aPosition.xyz *= aScale;
	aPosition.w = 1.0f;

	Matrix mat = mul(aWorldMatrix, ViewProjection);
	output.Position = mul(aPosition, mat);
	output.Tex = aTex;

	output.Normal = mul(float4(aNormal, 0), aWorldMatrix).xyz;
	output.BiNormal = aBiNormal;
	output.Tangent = mul(float4(aTangent, 0), aWorldMatrix).xyz;
	output.WorldPosition = mul(aPosition, aWorldMatrix);

	return output;
}

PBLVertexShaderData CalcVertexShaderData_Instanced_Tree(float4 aPosition, float3 aNormal
, float2 aTex, float3 aBiNormal, float3 aTangent, Matrix aWorldMatrix, float3 aScale, float aHeight)
{
	PBLVertexShaderData output = (PBLVertexShaderData)0;
	aPosition.xyz *= aScale;
	aPosition.w = 1.0f;

	output.Position = mul(aPosition, aWorldMatrix);

	float displacement = output.Position.y - aHeight;
	displacement /= 30.0f;
	displacement = saturate(displacement);

	float magnitude1 = 0.025f * 0.33f * 0.75f;
	float magnitude2 = 0.0123456f * 0.33f * 0.75f;

	output.Position += sin(TotalTime - output.Position.x * 0.1f) * magnitude1 * displacement - magnitude1 * displacement;

	output.Position += sin(TotalTime - output.Position.x * 0.1f) * magnitude2 * displacement - magnitude2 * displacement;

	output.Position = mul(output.Position, ViewProjection);
	output.Tex = aTex;

	output.Normal = mul(float4(aNormal, 0), aWorldMatrix).xyz;
	output.BiNormal = aBiNormal;
	output.Tangent = mul(float4(aTangent, 0), aWorldMatrix).xyz;
	output.WorldPosition = mul(aPosition, aWorldMatrix);

	return output;
}

float3 ReflectionFresnel(const float3 aSubstance, const float3 aLightDir, const float3 aHalfVec, float aRoughness)
{
	float LdotH = dot(aLightDir, aHalfVec);
	LdotH = saturate(LdotH);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1.f - aSubstance);
	fresnel = fresnel / (2 - 1 * aRoughness);
	fresnel = aSubstance + fresnel;
	return fresnel;

	//return (aSubstance + (1.f - aSubstance) * pow((1.f - saturate(dot(aLightDir, aHalfVec))), 5) / (4 - 3 * aRoughness));
}

float RoughToSPow(float fRoughness)
{
	return (2.f / (fRoughness * fRoughness)) - 2.f;
}

float GetSpecPowToMip(float fSpecPow, int nMips)
{
	float fSmulMaxT = (exp2(-10.0 / sqrt(fSpecPow)) - k0) / k1;

	return float(nMips - 1 - 0) * (1.0 - clamp(fSmulMaxT / fMaxT, 0.0, 1.0));
}

float3 Fresnel(const float3 aSubstance, const float3 aLightDir, const float3 aHalfVec)
{
	float LdotH = dot(aLightDir, aHalfVec);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1.f - aSubstance);
	fresnel = aSubstance + fresnel;
	//return (aSubstance + (1.f - aSubstance) * pow((1.f - saturate(dot(aLightDir, aHalfVec))), 5));
	return fresnel;


}

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


float3 SSAOGetViewPosition(float2 uv)
{
	float depth = DepthTexture.Sample(pointSample, uv).x;

	float x = uv.x * 2.f - 1.f;
	float y = (1.f - uv.y) * 2.f - 1.f;
	float z = depth;

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	return worldPosition.xyz;
}

float3 SSAOGetWorldPosition(float2 uv)
{
	float3 worldPosition = SSAOGetViewPosition(uv);
	worldPosition = mul(worldPosition, NotInvertedView);
	return worldPosition.xyz;
}


float4 SSAOGetRandom(float2 uv)
{
	float4 random = RandomTexture.Sample(pointSampleWrap, uv * g_screen_size / random_size);
	random *= 2.0f;
	random -= 1.f;
	normalize(random);
	return random;
}

float DoAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = SSAOGetWorldPosition(tcoord + uv) - p;
	const float3 v = normalize(diff);
	const float d = length(diff)*SSAO_Scale;
	return max(0.0,dot(cnorm,v)-SSAO_Bias)*(1.0/(1.0+d))*SSAO_Intensity;
}

float CalculateSSAO(float2 aUV, float3 aNormal, float3 aWorldPosition)
{
	const float2 vec[4] = {float2(1,0),float2(-1,0),
			float2(0,1),float2(0,-1)};

	float3 p = SSAOGetWorldPosition(aUV);// aWorldPosition;
	float3 pView = SSAOGetViewPosition(aUV);
	float3 n = aNormal;

	float2 rand = SSAOGetRandom(aUV ).xy;

	float ao = 0.0f;
	float rad = SSAO_Sample_Size/pView.z; // SKALL VARA I VIEWSPACE

	//**SSAO Calculation**//
	int iterations = 4;
	for (int j = 0; j < iterations; ++j)
	{
	  	float2 coord1 = reflect(vec[j],rand)*rad;
  		float2 coord2 = float2(coord1.x*0.707 - coord1.y*0.707,
			  coord1.x*0.707 + coord1.y*0.707);
  
 	 	ao += DoAmbientOcclusion(aUV, coord1*0.25, p, n);
 	 	ao += DoAmbientOcclusion(aUV, coord2*0.5, p, n);
 	 	ao += DoAmbientOcclusion(aUV, coord1*0.75, p, n);
 	 	ao += DoAmbientOcclusion(aUV, coord2, p, n);
	}
	ao /= (float)iterations*4.0;
	ao = 1.f- ao;
	return ao;
}