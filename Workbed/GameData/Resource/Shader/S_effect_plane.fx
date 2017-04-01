#include "S_variables_shader.fx"


PS_INPUT_POS_COL VS(VS_INPUT_POS_COL input)
{
	PS_INPUT_POS_COL output = (PS_INPUT_POS_COL)0;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, ViewProjection);
	
	output.Color = input.Color;
	return output;
}

float4 PS(PS_INPUT_POS_COL input) : SV_Target
{
	return input.Color;
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