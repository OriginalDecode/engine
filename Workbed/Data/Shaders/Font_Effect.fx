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

PS_INPUT_POS_COL_UV VS_2(VS_INPUT_POS_COL_UV input)
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

static const float2 offset = float2(1.f / 256.f, (1.f / 256.f));

float4 PS_2(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 color = AlbedoTexture.Sample(linearSample_Clamp, input.UV).aaaa;
	color.rgba *= input.Color.rgba;
	return color;
};


float4 PS(PS_INPUT_POS_COL_UV input) : SV_Target
{
	float4 color = AlbedoTexture.Sample(linearSample_Clamp, input.UV).rgba;

	float4 n = AlbedoTexture.Sample(linearSample_Clamp, float2(input.UV.x, input.UV.y - offset.y ));

	float4 e = AlbedoTexture.Sample(linearSample_Clamp, float2(input.UV.x + offset.x, input.UV.y));

	float4 s = AlbedoTexture.Sample(linearSample_Clamp, float2(input.UV.x, input.UV.y + offset.y));

	float4 w = AlbedoTexture.Sample(linearSample_Clamp, float2(input.UV.x - offset.x, input.UV.y));

	//return n;
	color += (n + e + s + w);
	//color.rgba *= input.Color.rgba;
	return color;
};

technique11 Render
{
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
	
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2()));
	}	
}
