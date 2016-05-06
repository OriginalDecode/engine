#include "SharedVariables.fx"

PS_INPUT_POS_UV VS(VS_INPUT_POS_UV input)
{
	PS_INPUT_POS_UV output = (PS_INPUT_POS_UV)0;
	output.UV = input.UV;
	output.Pos = input.Pos;
	
	return output;
}

float4 PS(PS_INPUT_POS_UV input) : SV_Target
{
	return DiffuseTexture.Sample(linearSample_Clamp, input.UV);
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