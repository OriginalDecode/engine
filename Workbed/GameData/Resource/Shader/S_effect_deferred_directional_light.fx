#include "S_variables_pbl.fx"
#include "S_light_directional.fx"

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

	float3 toLight = -DirectionalLight[0].Direction.xyz;
	float3 lightDir = normalize(toLight);
	float3 halfVec = normalize(lightDir + toEye.xyz);
	float NdotL = saturate(dot(pixelData.myNormal.xyz, lightDir));
	float HdotN = saturate(dot(halfVec, pixelData.myNormal.xyz));
	float NdotV = saturate(dot(pixelData.myNormal.xyz, toEye));

	float3 F = saturate(Fresnel(pixelData.mySubstance.xyz, lightDir, halfVec));
	float D = saturate(D_GGX(HdotN, (pixelData.myRoughness + 1.f) / 2.f));
	float V = saturate(V_SchlickForGGX((pixelData.myRoughness + 1.f) / 2.f, NdotV, NdotL));
	float lambert = NdotL;

	float3 lightColor = DirectionalLight[0].Color.xyz * DirectionalLight[0].Color.w;
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
		//SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}