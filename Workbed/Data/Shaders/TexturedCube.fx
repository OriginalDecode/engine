#include "SharedVariables.fx"

PS_INPUT_POS_NORM_UV VS(VS_INPUT_POS_NORM_UV input)
{
	PS_INPUT_POS_NORM_UV output = (PS_INPUT_POS_NORM_UV)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	return output;
}

float4 PS(PS_INPUT_POS_NORM_UV input) : SV_Target
{

	float4 color = AlbedoTexture.Sample(linearSample_Wrap, input.UV);
	color.r = 1;
	color.g = 1;
	color.b = 1;
	float ambient = 1.f;
	float4 ambientDiffuse = ambient * color;
	return float4(ambientDiffuse);
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
