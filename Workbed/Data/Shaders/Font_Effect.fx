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

RasterizerState MyWireframe
{
	FillMode = Wireframe;
};

RasterizerState Solid
{
	FillMode = SOLID;
};

float4 Text_PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 text = FontTexture.Sample(linearSample_Clamp, input.UV).rrrr;
	return text;
};

float4 Outline_PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 outline = FontTexture.Sample(linearSample_Clamp, input.UV).aaaa;
	
	outline.rgb = float3(0,0,0);
	return outline;
};

float4 Wireframe_PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	return float4(1,0,0,1);;
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
	
	//pass Wireframe
	//{
	//	SetVertexShader(CompileShader(vs_5_0, VS()));
	//	SetGeometryShader(NULL);
	//	SetPixelShader(CompileShader(ps_5_0, Wireframe_PS()));
	//	SetRasterizerState(MyWireframe);
	//}
	
}