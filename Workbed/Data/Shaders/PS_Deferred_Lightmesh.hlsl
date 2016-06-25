//---------------------------------
//	Deferred Lightmesh Pixel Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Pointlight : register(b0)
{
	row_major float4x4 View; 
	row_major float4x4 InvertedProjection; 
	float4 color;
	float4 position;
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
	float range : RANGE;
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
	float fallOff = 1.f - (someDistance / (someRange + 0.1f));
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
float4 PS(VS_OUTPUT input) : SV_Target
{
	input.uv /= input.uv.w;
	float2 texCoord = input.uv.xy;

	float4 albedo = AlbedoTexture.Sample(point_Clamp, texCoord);
	float4 normal = NormalTexture.Sample(point_Clamp, texCoord);
	float4 depth = DepthTexture.Sample(point_Clamp, texCoord);
	normal.xyz *= 2.0f;
	normal.xyz -= 1.f;

	float x = texCoord.x * 2.f - 1.f;
	float y = (1.f - texCoord.y) * 2.f - 1.f;
	float z = depth.x;

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, View);


	//PointLight-Calc
	float3 lightVec = position - worldPosition;
	return float4(lightVec.rgb,1);
	float distance = length(lightVec);
	lightVec = normalize(lightVec);

	float lambert = dot(lightVec, normal);

	float3 lightColor = 0.f;
	float intensity = color.w;

	lightColor = saturate(lambert * color); 
	float4 finalColor = float4(lightColor * intensity, 1.f);
	finalColor *= albedo;
	finalColor.a = 1.f;
	return saturate(finalColor * CalculateTotalAttenuation(distance, input.range));
};