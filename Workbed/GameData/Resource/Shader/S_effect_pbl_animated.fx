#include "S_variables_pbl.fx"
#include "S_light_directional.fx"
#include "S_light_point.fx"
#include "S_light_spot.fx"

matrix Bones[32];

struct VS_ANIMATION_INPUT
{
	float4 Position : POSITION;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float4 BiNormal : BINORMAL;
	float4 Tangent : TANGENT;
	uint4 Bones : BONES;
	float4 Weights : WEIGHTS;
};

struct SkinnedVert
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};

struct VS_ANIMATION_INPUT_PROP
{
	float4 Position : POSITION;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float4 BiNormal : BINORMAL;
	float4 Tangent : TANGENT;
	uint4 Bones : BONES;
	float4 Weights : WEIGHTS;
	float4 Color : COLOR;
};

struct SkinnedVert_PROP
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float4 WorldPosition : POSITION;
	float4 DirectionalLightCoordinates : POSITION1;
};

SkinnedVert VS(VS_ANIMATION_INPUT input)
{
	SkinnedVert output = (SkinnedVert)0;

	input.Position.xyz *= Scale;
	input.Position.w = 1.0f;
	
	output.Tex = input.Tex;

	float4 pos = input.Position;
	float3 norm = input.Normal;
	float3 binorm = input.BiNormal;
	float3 tangent = input.Tangent;
	
	uint iBone;
	float fWeight;

	// Bone 0
	iBone = input.Bones.x;
	fWeight = input.Weights.x;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 1
	iBone = input.Bones.y;
	fWeight = input.Weights.y;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 2
	iBone = input.Bones.z;
	fWeight = input.Weights.z;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 3
	iBone = input.Bones.w;
	fWeight = input.Weights.w;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);


	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, ViewProjection);

	output.Normal = mul(output.Normal.xyz, World).xyz;
	output.BiNormal = mul(binorm, World).xyz;
	output.Tangent = mul(tangent, World).xyz;

	output.WorldPosition = mul(input.Position, World);

	return output;
}



SkinnedVert_PROP VS_PROP(VS_ANIMATION_INPUT_PROP input)
{
	SkinnedVert_PROP output = (SkinnedVert_PROP)0;

	input.Position.xyz *= Scale;
	input.Position.w = 1.0f;
	
	output.Tex = input.Tex;
	output.Tex2 = input.Tex2;
	output.Color = input.Color;

	float4 pos = input.Position;
	float3 norm = input.Normal;
	float3 binorm = input.BiNormal;
	float3 tangent = input.Tangent;
	
	uint iBone;
	float fWeight;

	// Bone 0
	iBone = input.Bones.x;
	fWeight = input.Weights.x;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);
	output.BiNormal += fWeight * mul(binorm, (float3x3)Bones[iBone]);
	output.Tangent += fWeight * mul(tangent, (float3x3)Bones[iBone]);

	// Bone 1
	iBone = input.Bones.y;
	fWeight = input.Weights.y;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);
	output.BiNormal += fWeight * mul(binorm, (float3x3)Bones[iBone]);
	output.Tangent += fWeight * mul(tangent, (float3x3)Bones[iBone]);

	// Bone 2
	iBone = input.Bones.z;
	fWeight = input.Weights.z;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);
	output.BiNormal += fWeight * mul(binorm, (float3x3)Bones[iBone]);
	output.Tangent += fWeight * mul(tangent, (float3x3)Bones[iBone]);

	// Bone 3
	iBone = input.Bones.w;
	fWeight = input.Weights.w;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);
	output.BiNormal += fWeight * mul(binorm, (float3x3)Bones[iBone]);
	output.Tangent += fWeight * mul(tangent, (float3x3)Bones[iBone]);


	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, ViewProjection);

	output.Normal = mul(output.Normal.xyz, World).xyz;
	output.BiNormal = mul(input.BiNormal.xyz, World).xyz;
	output.Tangent = mul(input.Tangent, World).xyz;
	output.Normal = float4(0.f, 0.f, 0.f, 1.f);
	output.WorldPosition = mul(input.Position, World);

	output.DirectionalLightCoordinates = mul(output.WorldPosition, ShadowMVP);
	output.DirectionalLightCoordinates.xyz = output.DirectionalLightCoordinates.xyz / output.DirectionalLightCoordinates.w;
	output.DirectionalLightCoordinates.y = output.DirectionalLightCoordinates.y * -1.f;
	output.DirectionalLightCoordinates.xy = output.DirectionalLightCoordinates.xy + 1;
	output.DirectionalLightCoordinates.xy = output.DirectionalLightCoordinates.xy * 0.5f;

	//output.DirectionalLightCoordinates /= output.DirectionalLightCoordinates.w;
	output.DirectionalLightCoordinates.z -= 0.00001;

	return output;
}

float4 PS_PROP_DEPTHONLY(SkinnedVert_PROP input) : SV_TARGET
{
	return float4(1, 0,0,1);
}

float4 PS_EMISSIVE_DEPTHONLY(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
return float4(1, 0,0,1);
}

float4 PS_NONE_DEPTHONLY(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
	return float4(1, 0,0,1);
}

float4 PS_FUNCTION_DEPTHONLY(SkinnedVert input) : SV_Target
{
	return float4(1, 0,0,1);
}

GBuffer PS_FUNCTION(SkinnedVert input) : SV_Target
{
	return CalculateGBuffer(input.Position, input.Tex, input.Normal, input.BiNormal, input.Tangent);
}

GBuffer PS_PROP(SkinnedVert_PROP input) : SV_TARGET
{
	return CalculateGBuffer(input.Position, input.Tex, input.Normal, input.BiNormal, input.Tangent);
}

GBuffer PS_EMISSIVE(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
	return PS_FUNCTION(input);
}

GBuffer PS_NONE(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
	return CalculateGBuffer(input.Position, input.Tex, input.Normal, input.BiNormal, input.Tangent);
}

technique11 Render_1UVSET
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_FUNCTION()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_1UVSET_DEPTHONLY
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_FUNCTION_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_2UVSET_COLOR
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS_PROP()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_PROP()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_2UVSET_COLOR_DEPTHONLY
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS_PROP()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_PROP_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_FUNCTION()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}