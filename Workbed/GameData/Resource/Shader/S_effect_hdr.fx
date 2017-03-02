#include "S_variables_shader.fx"

Texture2D Original;
Texture2D AverageColor;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float LinearToLum(float4 aColor)
{
	return(aColor.r*0.3+aColor.g*0.59+aColor.b*0.11);
}

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}

float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 pixelColor = Original.Sample(linearSampling, input.Tex);
	
	float LuminancePixel = LinearToLum(pixelColor);
	float AverageLuminance = AverageColor.Sample(linearSampling, input.Tex);

	float LP = LuminancePixel * 0.5f / AverageLuminance;

	pixelColor = pixelColor * (LP / (1.f + LP));
	return pixelColor;
}


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}