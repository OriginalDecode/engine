#include "SharedVariables.fx"

PS_INPUT_POS_COL_UV VS(VS_INPUT_POS_COL_UV input)
{
	PS_INPUT_POS_COL_UV output = (PS_INPUT_POS_COL_UV)0;
	
	output.Pos.x *= Scale.x;
	output.Pos.y *= Scale.y;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	
	output.Pos.x += Position.x;
	output.Pos.y += Position.y; 	
	
	output.Pos = mul(output.Pos, Projection);
	output.Color = input.Color;	
	output.UV = input.UV;
	return output;
};

float4 Text_PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 text = FontTexture.Sample(linearSample_Clamp, input.UV).rrrr;
	//text = float4(1,0,0,1);
	return text;
};

float4 Outline_PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 outline = FontTexture.Sample(linearSample_Clamp, input.UV).aaaa;
	outline.rgb = float3(0,0,0);
	//outline.a = 0.f;
	return outline;
};

technique11 Render
{	
	pass Outline
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, Outline_PS()));
	}	

	pass Text
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, Text_PS()));
	}		
}