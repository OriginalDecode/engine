#include "SharedVariables.fx"

PS_INPUT_POS_UV VS(VS_INPUT_POS_UV input)
{
	PS_INPUT_POS_UV output = (PS_INPUT_POS_UV)0;
	
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos.x *= Scale.x;
	output.Pos.y *= Scale.y;
	output.Pos.x += Position.x;
	output.Pos.y += Position.y;	
	output.Pos = mul(output.Pos, Projection);
	output.UV = input.UV;
	return output;
};

float4 PS(PS_INPUT_POS_UV input) : SV_Target
{
	float4 color = AlbedoTexture.Sample(linearSample_Wrap ,input.UV);	
	return color;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
