#include "S_variables_pbl.fx"

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}


float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	DeferredPixelData pixelData = CalculateDeferredPixelData(input.Tex.xy);

	float3 viewPos = cameraPosition;
	float3 toEye = normalize(viewPos - pixelData.myWorldPosition.xyz);
	float ao = CalculateSSAO(input.Tex, pixelData.myNormal.xyz, pixelData.myWorldPosition.xyz);
	ao *= pixelData.myAmbientocclusion;

	//float ao = pixelData.myAmbientocclusion;

	float3 reflectionFresnel = ReflectionFresnel(pixelData.mySubstance.xyz, pixelData.myNormal
		, toEye, 1.f - pixelData.myRoughnessOffsetted);

	float3 ambientDiffuse = CubeMap.SampleLevel(linearSampling, pixelData.myNormal, 9).xyz
		* ao * pixelData.myMetalnessAlbedo * (1.f - reflectionFresnel);

	float3 reflectionVector = reflect(toEye, pixelData.myNormal);
	float fakeLysSpecularPower = RoughToSPow(pixelData.myRoughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, 12);

	float3 ambientSpec = CubeMap.SampleLevel(linearSampling, reflectionVector, lysMipMap).xyz
		* ao * reflectionFresnel;

	float3 finalColor = (ambientDiffuse + ambientSpec);

	const float fogStartDistanceFromCamera = 50.f; //Fog starts on this distance from camera
	const float fogEndDistanceFromCamera = 512.f; //Fog is full from here

	const float4 fogColor = Color;

	const float fogThickness = 0.6f;

	float4 moddedGrayColor = fogColor;
	float distanceToPixel = min(length(cameraPosition - pixelData.myWorldPosition.xyz), fogEndDistanceFromCamera);
	distanceToPixel -= fogStartDistanceFromCamera;
	distanceToPixel = max(distanceToPixel, 0.f);


	float grayDistance = min(distanceToPixel / (fogEndDistanceFromCamera - fogStartDistanceFromCamera), fogThickness);
	finalColor = lerp(finalColor, moddedGrayColor, grayDistance);

	finalColor.xyz += pixelData.myEmissive.xyz * emissiveMultiplier;
	finalColor.xyz *= float3(1.32, 1.25, 1.1);
	return float4(finalColor, 1.f);

}

float4 PS_CHEAP(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(pointSample, input.Tex.xy);
	albedo.xyz *= float3(0.69f, 0.58f, 0.39f);
	float4 emissiveSample = EmissiveTexture.Sample(pointSample, input.Tex.xy);
	return float4(albedo.xyz * emissiveSample.w , 1.f);
}

float4 PS_CHEAP_NIGHTMARE(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(pointSample, input.Tex.xy);
	albedo.xyz *= float3(0.69f, 0.58f, 0.39f);
	float4 emissiveSample = EmissiveTexture.Sample(pointSample, input.Tex.xy);
	return float4(albedo.xyz * emissiveSample.w , 1.f);
}


BlendState BlendCorrect
{
	BlendEnable[0] = FALSE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState BlendAlpha
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 Render
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_Cheap
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_CHEAP()));
		SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_Cheap_Nightmare
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_CHEAP_NIGHTMARE()));
		SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}
