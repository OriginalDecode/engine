#include "S_variables_shader.fx"


SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}

float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(linearSampling, input.Tex);
	color.w = 1.f;
	return color;
}

float4 PS_Partilce(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(linearSampling, input.Tex); 
	return color;
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

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}

technique11 Render_Particle
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Partilce()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}