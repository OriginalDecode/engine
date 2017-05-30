cbuffer SSAO : register(b0)
{
	row_major float4x4 InvertedProjection;
	row_major float4x4 InvertedView;
}

Texture2D DiffuseTexture		: register ( t0 );
Texture2D NormalTexture 	: register ( t1 );
Texture2D DepthTexture 		: register ( t7 );
SamplerState SSAOSampler 	: register ( s0 );

struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

static const float SSAO_Sample_Size = 0.5f; //Size of the SSAO
static const float SSAO_Intensity = 0.7f; //The intensity of the SSAO
static const float SSAO_Scale = 0.1f;
static const float SSAO_Bias = 0.15f;

float3 SSAOGetViewPosition(float2 uv)
{
	float depth = DepthTexture.Sample(SSAOSampler, uv).x;

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
	worldPosition = mul(worldPosition, InvertedView);
	return worldPosition.xyz;
}

/*
float4 SSAOGetRandom(float2 uv)
{
	float4 random = RandomTexture.Sample(pointSampleWrap, uv * g_screen_size / random_size);
	random *= 2.0f;
	random -= 1.f;
	normalize(random);
	return random;
}*/

float DoAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = SSAOGetWorldPosition(tcoord + uv) - p;
	const float3 v = normalize(diff);
	const float d = length(diff)*SSAO_Scale;
	return max(0.0,dot(cnorm,v)-SSAO_Bias)*(1.0/(1.0+d))*SSAO_Intensity;
}

float CalculateSSAO(float2 aUV, float3 aNormal)
{
	const int iterations = 4;
	const float2 vec[iterations] = {float2(1,0),float2(-1,0),
			float2(0,1),float2(0,-1)};

	float3 p = SSAOGetWorldPosition(aUV);// aWorldPosition;
	float3 view_pos = SSAOGetViewPosition(aUV);
	float3 n = aNormal;

	half2 rand = float2(0.5h, 0.5h); //SSAOGetRandom(uv).xy;

	float ao = 0.0f;
	float rad = SSAO_Sample_Size / view_pos.z; // SKALL VARA I VIEWSPACE

	//**SSAO Calculation**//
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
	ao /= (float)4*4.0;
	ao = 1.f- ao;

	return ao;
}

struct ReturnTextures
{
	float4 m_Diffuse;
	float4 m_SSAODebug;
};

ReturnTextures PS(VS_OUTPUT input) : SV_Target
{
	float4 normal = NormalTexture.Sample(SSAOSampler, input.uv) * 2 - 1;
	float ao = CalculateSSAO(input.uv,normal);

	ReturnTextures return_textures;
	return_textures.m_Diffuse  = DiffuseTexture.Sample(SSAOSampler, input.uv);
	return_textures.m_SSAODebug = float4(ao,ao,ao,1);

	return return_textures;
}