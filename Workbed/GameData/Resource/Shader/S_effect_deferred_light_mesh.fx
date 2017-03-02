#include "S_variables_pbl.fx"
#include "S_light_point.fx"

PS_LIGHTMESH VS(VS_LIGHTMESH input)
{
	PS_LIGHTMESH output = (PS_LIGHTMESH)0;

	float4 scale = float4(PointLights[0].Range,PointLights[0].Range, PointLights[0].Range, 1.0f);
	input.Position *= scale;
	
	input.Position.w = 1.0f;

	Matrix mat = mul(World, ViewProjection);
	output.Position = mul(input.Position, mat);

	float x = output.Position.x;
	float y = output.Position.y;
	float w = output.Position.w;

	output.Tex = float4((float2(x + w, w - y)) / 2, output.Position.zw);
	
	return output;
}


float4 PS(PS_LIGHTMESH input) : SV_Target
{
	input.Tex /= input.Tex.w;
	DeferredPixelData pixelData = CalculateDeferredPixelData(input.Tex.xy);

	float3 viewPos = cameraPosition;
	float3 toEye = normalize(viewPos - pixelData.myWorldPosition.xyz);



	float3 toLight = PointLights[0].Position - pixelData.myWorldPosition;
	float3 lightDir = normalize(toLight); //Ska denna negeras? Vi gjorde det innan (i RTS't) men gör det inte i matten below

	float3 halfVec = normalize(lightDir + -toEye.xyz);

	float attenuation = Attenuation(toLight, PointLights[0].Range);
	float NdotL = saturate(dot(pixelData.myNormal, toLight));
	float HdotN = saturate(dot(halfVec, pixelData.myNormal));

	float3 F = saturate(Fresnel(pixelData.mySubstance, lightDir, halfVec));
	float D = saturate(D_GGX(HdotN, (pixelData.myRoughness + 1) / 2));
	float V = saturate(V_SchlickForGGX((pixelData.myRoughness + 1) / 2, dot(pixelData.myNormal, toEye), NdotL));


	float3 lightColor = PointLights[0].Color.xyz * PointLights[0].Color.w;
	float3 directSpec = (((D * V * F) / 3.14159 + (1 - F) * NdotL 
		* pixelData.myMetalnessAlbedo.xyz * pixelData.myAmbientocclusion.xyz)) * lightColor * attenuation;

	float lambert = dot(lightDir, pixelData.myNormal);
	//float3 finalColor = saturate(lambert * lightColor * attenuation);

	float intensity = PointLights[0].Color.w;
	float4 finalColor = PointLights[0].Color * intensity;
	finalColor.w = 1.f;
	finalColor = saturate(lambert * finalColor);
	finalColor *= intensity;
	finalColor *= pixelData.myAlbedo;
	finalColor *= attenuation;
	//finalColor.w = 1.f;
	return saturate(finalColor);
}

BlendState Blend
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
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}