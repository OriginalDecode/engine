#include "S_variables_shader.fx"

float bloomValue = 1.0;


SamplerState bilinearSampling
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
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
	float4 color = DiffuseTexture.Sample(bilinearSampling, input.Tex);
	float4 fogOfWar = FogOfWarTexture.Sample(bilinearSampling, input.Tex);

	return color * fogOfWar;
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