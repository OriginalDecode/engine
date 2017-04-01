#include "S_variables_pbl.fx"
#include "S_light_directional.fx"
#include "S_light_point.fx"
#include "S_light_spot.fx"

float4 AmbientHue;
//float4 Color;
float GradiantValue;
float2 GradiantDirection;

PS_INPUT_POS_TEX VS_1UVSET(VS_INPUT_POS_TEX input)
{

	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, ViewProjection);
	output.Tex = input.Tex;

	return output;
}


float4 PS_1UVSET(PS_INPUT_POS_TEX input) : SV_Target
{

	//float2 colorPos;
	//colorPos.x = input.Tex.x * GradiantDirection.x;
	//colorPos.y = (1.f - input.Tex.y) * GradiantDirection.y;
	float4 color = AlbedoTexture.Sample(linearSampling, input.Tex);
	//color.xyz *= Color.xyz;
	//color.w *= GradiantValue;
	return color;
	//return float4(1.f - GradiantValue, GradiantValue, 0, 0.5f);

}


BlendState Blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState DisableDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

technique11 Render
{

	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWrites, 0);
	}

}



technique11 Render_1UVSET_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWrites, 0);
	}
}
