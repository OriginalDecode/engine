#include "S_variables_pbl.fx"
#include "S_light_spot.fx"

PS_LIGHTMESH VS(VS_LIGHTMESH input)
{
	PS_LIGHTMESH output = (PS_LIGHTMESH)0;

	float xyScale = tan(SpotLights[0].Angle) * SpotLights[0].Range;

	input.Position.x *= xyScale;
	input.Position.y *= xyScale;
	input.Position.z *= SpotLights[0].Range;
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

	float3 lightToPixel = normalize(pixelData.myWorldPosition.xyz - SpotLights[0].Position);
	float spotFactor = dot(lightToPixel, normalize(SpotLights[0].Direction));

	if(spotFactor < SpotLights[0].CosAngle)
	{
		discard;
	}

	float3 viewPos = cameraPosition;
	float3 toEye = normalize(viewPos - pixelData.myWorldPosition.xyz);

	float3 toLight = SpotLights[0].Position - pixelData.myWorldPosition;
	float3 lightDir = normalize(toLight);
	float3 halfVec = normalize(lightDir + toEye.xyz);
	float NdotL = saturate(dot(pixelData.myNormal.xyz, lightDir));
	float HdotN = saturate(dot(halfVec, pixelData.myNormal.xyz));
	float NdotV = saturate(dot(pixelData.myNormal.xyz, toEye));

	float3 F = saturate(Fresnel(pixelData.mySubstance.xyz, -lightDir, halfVec));
	float D = saturate(D_GGX(HdotN, (pixelData.myRoughness + 1.f) / 2.f));
	float V = saturate(V_SchlickForGGX((pixelData.myRoughness + 1.f) / 2.f, NdotV, NdotL));
	float lambert = NdotL;


	float linearAttenuation = Attenuation(toLight, SpotLights[0].Range);
	float angularAttenuation = (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - SpotLights[0].CosAngle));
	float attenuation = linearAttenuation * angularAttenuation;
	float3 lightColor = SpotLights[0].Color.xyz * 10 * attenuation;
	float3 directSpecc = F * D * V * NdotL * lightColor;
	
	return float4(directSpecc, 1.f);
}


BlendState BlendCorrect
{
	BlendEnable[0] = TRUE;
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
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}