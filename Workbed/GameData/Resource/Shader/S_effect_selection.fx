#include "S_variables_shader.fx"
#include "S_light_directional.fx"

float4 AmbientHue;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

PS_INPUT_POS_NORM_TEX_BI_TANG VS(VS_INPUT_POS_NORM_TEX_BI_TANG_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	PS_INPUT_POS_NORM_TEX_BI_TANG output = (PS_INPUT_POS_NORM_TEX_BI_TANG)0;
	input.Position.xyz *= input.scale;
	output.Position = mul(input.Position, locWorld);
	output.Position = mul(output.Position, ViewProjection);
	
	output.Tex = input.Tex;
	
	
	output.Normal = mul(float4(input.Normal, 0), locWorld).xyz;
	output.BiNormal = input.BiNormal;
	output.Tangent = mul(float4(input.Tangent, 0), locWorld).xyz;

	output.WorldPosition = mul(input.Position, locWorld);
	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
	return AlbedoTexture.Sample(linearSampling, input.Tex);;
}

float4 PS_DEPTHONLY(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
return float4(0, 0, 0, 0);
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

technique11 Render_1UVSET_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_1UVSET_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_DEPTHONLY()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}