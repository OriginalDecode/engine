



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
	float3 view_pos = SSAOGetViewPosition(aUV);
	float3 n = aNormal;

	float2 rand = SSAOGetRandom(uv).xy;

	float ao = 0.0f;
	float rad = SSAO_Sample_Size / view_pos.z; // SKALL VARA I VIEWSPACE

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