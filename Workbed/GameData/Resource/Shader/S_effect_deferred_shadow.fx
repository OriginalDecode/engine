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
	float depth = DepthTexture.Sample(pointSample, input.Tex).x;
	float x = input.Tex.x * 2.f - 1.f;
	float y = (1.f - input.Tex.y) * 2.f - 1.f;
	float z = depth;

	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, NotInvertedView);

	float4 shadowVector = mul(worldPosition, ShadowMVP);
	shadowVector.xyz = shadowVector.xyz / shadowVector.w;
	shadowVector.y = shadowVector.y * -1.f;
	shadowVector.xy = shadowVector.xy + 1;
	shadowVector.xy = shadowVector.xy * 0.5f;

	float4 finalColor = AlbedoTexture.Sample(pointSample, input.Tex);

	float2 samplePoint = shadowVector.xy;
	float sampleValue = ShadowDepth.Sample(linearSampling, samplePoint).x;
	float compareValue = shadowVector.z;
	if (sampleValue < compareValue - shadowEpsilon)
	{
		if(samplePoint.x > 0 && samplePoint.x < 1 && samplePoint.y > 0 && samplePoint.y < 1)
		{
			finalColor.xyz *= float3(0.60f, 0.61f, 0.70f);
		}
	}



	return finalColor;
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
	BlendEnable[0] = false;
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
