#include "S_variables_pbl.fx"
#include "S_light_directional.fx"
#include "S_light_point.fx"
#include "S_light_spot.fx"

float4 AmbientHue;



PS_INPUT_1UVSET VS_1UVSET(VS_INPUT_1UVSET_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	PBLVertexShaderData vertexData = CalcVertexShaderData_Instanced(input.Position, input.Normal
	, input.Tex, input.BiNormal, input.Tangent, locWorld, input.scale);

	PS_INPUT_1UVSET output = (PS_INPUT_1UVSET)0;
	output.Position = vertexData.Position;
	output.Normal = vertexData.Normal;
	output.Tex = vertexData.Tex;
	output.BiNormal = vertexData.BiNormal;
	output.Tangent = vertexData.Tangent;
	output.WorldPosition = vertexData.WorldPosition;
	
	return output;
}


PS_INPUT_2UVSET VS_2UVSET(VS_INPUT_2UVSET_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	PBLVertexShaderData vertexData = CalcVertexShaderData_Instanced(input.Position, input.Normal
	, input.Tex, input.BiNormal, input.Tangent, locWorld, input.scale);

	PS_INPUT_2UVSET output = (PS_INPUT_2UVSET)0;
	output.Position = vertexData.Position;
	output.Normal = vertexData.Normal;
	output.Tex = vertexData.Tex;
	output.BiNormal = vertexData.BiNormal;
	output.Tangent = vertexData.Tangent;
	output.WorldPosition = vertexData.WorldPosition;
	output.Tex2 = input.Tex2;

	return output;
}

PS_INPUT_1UVSET_COLOR VS_1UVSET_COLOR(VS_INPUT_1UVSET_COLOR_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	PBLVertexShaderData vertexData = CalcVertexShaderData_Instanced(input.Position, input.Normal
	, input.Tex, input.BiNormal, input.Tangent, locWorld, input.scale);

	PS_INPUT_1UVSET_COLOR output = (PS_INPUT_1UVSET_COLOR)0;
	output.Position = vertexData.Position;
	output.Normal = vertexData.Normal;
	output.Tex = vertexData.Tex;
	output.BiNormal = vertexData.BiNormal;
	output.Tangent = vertexData.Tangent;
	output.WorldPosition = vertexData.WorldPosition;
	output.Color = input.Color;
	return output;
}

PS_INPUT_2UVSET_COLOR VS_2UVSET_COLOR(VS_INPUT_2UVSET_COLOR_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	PBLVertexShaderData vertexData = CalcVertexShaderData_Instanced(input.Position, input.Normal
	, input.Tex, input.BiNormal, input.Tangent, locWorld, input.scale);

	PS_INPUT_2UVSET_COLOR output = (PS_INPUT_2UVSET_COLOR)0;
	output.Position = vertexData.Position;
	output.Normal = vertexData.Normal;
	output.Tex = vertexData.Tex;
	output.BiNormal = vertexData.BiNormal;
	output.Tangent = vertexData.Tangent;
	output.WorldPosition = vertexData.WorldPosition;
	output.Tex2 = input.Tex2;
	output.Color = input.Color;
	return output;
}

float4 PS_1UVSET(PS_INPUT_1UVSET input) : SV_Target
{
	return AlbedoTexture.Sample(linearSampling, input.Tex);
}

float4 PS_1UVSET_DEPTHONLY(PS_INPUT_1UVSET input) : SV_Target
{
	return float4(1.f, 1.f, 1.f, 1.f);
}

float4 PS_2UVSET(PS_INPUT_2UVSET input) : SV_Target
{
	return AlbedoTexture.Sample(linearSampling, input.Tex);
}

float4 PS_2UVSET_DEPTHONLY(PS_INPUT_2UVSET input) : SV_Target
{
	return float4(1.f, 1.f, 1.f, 1.f);
}

float4 PS_1UVSET_COLOR(PS_INPUT_1UVSET_COLOR input) : SV_Target
{
	return AlbedoTexture.Sample(linearSampling, input.Tex);
}

float4 PS_1UVSET_COLOR_DEPTHONLY(PS_INPUT_1UVSET_COLOR input) : SV_Target
{
	return float4(1.f, 1.f, 1.f, 1.f);
}

float4 PS_2UVSET_COLOR(PS_INPUT_2UVSET_COLOR input) : SV_Target
{
	return AlbedoTexture.Sample(linearSampling, input.Tex);
}

float4 PS_2UVSET_COLOR_DEPTHONLY(PS_INPUT_2UVSET_COLOR input) : SV_Target
{
	return float4(1.f, 1.f, 1.f, 1.f);
}

technique11 Render
{
/*
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
	}
	*/
}

technique11 Render_1UVSET_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_1UVSET_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_2UVSET_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2UVSET()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_2UVSET_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2UVSET_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_1UVSET_COLOR_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET_COLOR()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET_COLOR()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_1UVSET_COLOR_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET_COLOR()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET_COLOR_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_2UVSET_COLOR_INSTANCED
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2UVSET_COLOR()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2UVSET_COLOR()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}

technique11 Render_2UVSET_COLOR_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_2UVSET_COLOR()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_2UVSET_COLOR_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DepthDefault, 0);
	}
}