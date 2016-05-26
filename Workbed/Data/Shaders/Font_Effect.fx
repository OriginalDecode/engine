#include "SharedVariables.fx"

PS_INPUT_POS_COL_2UV VS(VS_INPUT_POS_COL_2UV input)
{
	PS_INPUT_POS_COL_2UV output = (PS_INPUT_POS_COL_2UV)0;
	
	output.Pos.x *= Scale.x;
	output.Pos.y *= Scale.y;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	
	
	output.Pos.x += Position.x;
	output.Pos.y += Position.y; 	
	
	output.Pos = mul(output.Pos, Projection);
	output.Color = input.Color;	
	output.UV = input.UV;
	output.UV2 = input.UV2;
	return output;
};

PS_INPUT_POS_COL_2UV VS_2(VS_INPUT_POS_COL_2UV input)
{
	PS_INPUT_POS_COL_2UV output = (PS_INPUT_POS_COL_2UV)0;
		
	output.Pos.x *= Scale.x;
	output.Pos.y *= Scale.y;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);

	output.Pos.x += Position.x;
	output.Pos.y += Position.y;	
	output.Pos = mul(output.Pos, Projection);
	output.Color = input.Color;	
	output.UV = input.UV;
	output.UV2 = input.UV2;
	return output;
};

static const float2 offset = float2(1.f / 256.f, (1.f / 256.f));

float4 PS_2(PS_INPUT_POS_COL_2UV input) : SV_Target
{
	float4 border = OutlineTexture.Sample(linearSample_Clamp, input.UV2).aaaa;
	float4 text = FontTexture.Sample(linearSample_Clamp, input.UV).aaaa;
	border.rgb = float3(0,0,0);
	//text *= input.Color;
	float4 color = (text+border);
	//color.rgba *= input.Color.rgba;
	return color;
};

technique11 Render
{	
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2()));
	}	
}
