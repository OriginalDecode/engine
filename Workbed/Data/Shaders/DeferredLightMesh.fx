#include "SharedVariables.fx"
#include "PointLight.fx"
//point sample clamp

PS_LIGHTMESH VS(VS_LIGHTMESH input)
{
	PS_LIGHTMESH output = (PS_LIGHTMESH)0;

	float4 scale = float4(PointLights[0].Range, PointLights[0].Range, PointLights[0].Range, 1.0f);
	input.Pos *= scale;

	input.Pos.w = 1.0f;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	float x = output.Pos.x;
	float y = output.Pos.y;
	float w = output.Pos.w;

	output.UV = float4((float2(x + w, w - y)) *0.5f, output.Pos.zw);

	return output;
}

static const float ambient = 0.42f;

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


float4 PS(PS_LIGHTMESH input) : SV_Target
{
	input.UV /= input.UV.w;
	float2 texCoord = input.UV.xy;

	float4 albedo = AlbedoTexture.Sample(pointSample_Clamp, texCoord);
	float4 normal = NormalTexture.Sample(pointSample_Clamp, texCoord);
	float4 depth = DepthTexture.Sample(pointSample_Clamp, texCoord);
	normal.xyz *= 2.0f;
	normal.xyz -= 1.f;

	float4 diffuse = ambient * albedo;

	float x = texCoord.x * 2.f - 1.f;
	float y = (1.f - texCoord.y) * 2.f - 1.f;
	float z = depth.x;

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, NotInvertedView);


	//PointLight-Calc
	float3 lightVec = PointLights[0].Position - worldPosition;

	float distance = length(lightVec);
	lightVec = normalize(lightVec);

	float lambert = dot(lightVec, normal);

	float3 lightColor = 0.f;
	float intensity = PointLights[0].Color.w;

	lightColor = saturate(lambert * PointLights[0].Color); 
	float4 finalColor = float4(lightColor * intensity, 1.f);
	finalColor *= albedo;
	finalColor.a = 1.f;
	return saturate(finalColor * CalculateTotalAttenuation(distance, PointLights[0].Range));
	//return float4(diffuse.rgb + (albedo.rgb * finalColor.rgb), 1);
}

BlendState Blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}