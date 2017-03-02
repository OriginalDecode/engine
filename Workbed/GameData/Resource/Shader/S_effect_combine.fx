#include "S_variables_shader.fx"

Texture2D SourceA;
Texture2D DepthA;
Texture2D SourceB;
Texture2D DepthB;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
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
	float depthA = DepthA.SampleLevel(linearSampling, input.Tex, 0).r;
	float depthB = DepthB.SampleLevel(linearSampling, input.Tex, 0).r;

	if(depthB < depthA)
	{
		return SourceB.SampleLevel(linearSampling, input.Tex, 0);
	}

	return SourceA.SampleLevel(linearSampling, input.Tex, 0);
}

float4 PS_NODEPTH(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 colorA = SourceA.Sample(linearSampling, input.Tex);
	float4 colorB = SourceB.Sample(linearSampling, input.Tex);
	
	//float4 color = saturate(colorB * colorB.w + colorA * colorA.w);
	float4 color = saturate(colorB + colorA);
	//color = saturate(color);

	color.w = 1.f;
	return color;
}

technique11 Render
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

technique11 Depth
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

technique11 NoDepth
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_NODEPTH()));
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}