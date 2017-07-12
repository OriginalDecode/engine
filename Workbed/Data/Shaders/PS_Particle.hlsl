//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

SamplerState point_sample : register ( s0 );
Texture2D ParticleTexture : register ( t0 );
Texture2D NormalTexture : register ( t1 );
Texture2D DepthTexture : register ( t8 );
//---------------------------------
//	Line3D Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer Matrices : register (b0)
{
	float4x4 view;
	float4x4 invProj;
	float4 view_dir; 
}

//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

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

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 depth = DepthTexture.Sample(point_sample, input.uv);
    float x = input.uv.x * 2.f - 1.f;
	float y = (1.f - input.uv.y) * 2 - 1;
	float z = depth.x;
	float4 worldpos = float4(x, y, z, 1.f);
	
	worldpos = mul(worldpos, invProj);
	worldpos = worldpos / worldpos.w;
	worldpos = mul(worldpos, view);

	float3 tang = float3(0,0,1);
	float3 normal = NormalTexture.Sample(point_sample, input.uv).rgb * 2 - 1;
	float3 nnormal = normalize(normal.rgb);

	float4 light_pos = float4(10,2,10,1);

	float4 toLight = (worldpos- light_pos );


	float3 binorm = normalize(cross(float3(normal.rgb), tang));
	float3x3 tangSpaceMatrix = float3x3(tang, binorm, nnormal);
	normal = normalize(mul(normal.rgb, tangSpaceMatrix));

	normal += 1;
	normal *= 0.5;



	float4 up = float4(0,1,0,1);
	// float3 right = cross(up, float3(view_dir.xyz));

	// normal = normalize(cross(right, view_dir));

	float ln = length(toLight);
	float attenuation = CalculateTotalAttenuation(ln, 10.f);
	float4 diffuse = ParticleTexture.Sample(point_sample, input.uv);
	float3 light_dir = float3(0,0,1);
	float4 color = float4(1,0,0,1);
	float NdotL = dot(normal, -light_dir);



	float3 output = float3(diffuse.rgb) * NdotL;
	float intensity = 2;
	float4 out_col = float4(output.rgb, diffuse.a) * float4(1,0,0,1);

	return out_col * intensity ;
};
