#include "SharedVariables.fx"

PARTICLE_PS_INPUT VS(PARTICLE_VS_INPUT input)
{
	PARTICLE_PS_INPUT output = (PARTICLE_PS_INPUT)0;

	output.Alpha = input.Alpha;
	output.Size = input.Size;

	output.Pos = float4(input.Pos.xyz, 1.f);
	output.Pos = mul(output.Pos, World);
	output.Pos = mul(output.Pos, View);

	return output;
}

cbuffer createQuad
{
	float4 quadPos[4] =
	{
		float4(-1,-1,0,0),
		float4(-1,1,0,0),
		float4(1,-1,0,0),
		float4(1,1,0,0),
	};

	float2 quadUV[4] =
	{
		float2(0,1),
		float2(0,0),
		float2(1,1),
		float2(1,0),
	};
};

[maxvertexcount(4)]
void GS(point PARTICLE_PS_INPUT input[1], inout TriangleStream<PARTICLE_PS_INPUT> triStream)
{
	PARTICLE_PS_INPUT output = (PARTICLE_PS_INPUT)0;

	for (int i = 0; i < 4; ++i)
	{
		output.Pos = (quadPos[i] * input[0].Size * 2) + input[0].Pos;
	
		output.Pos = mul(output.Pos, Projection);
		
		output.WorldViewProj = output.Pos;

		output.UV.x = quadUV[i].x;
		output.UV.y = quadUV[i].y;

		output.Alpha = input[0].Alpha;

		float x = output.Pos.x;
		float y = output.Pos.y;
		float w = output.Pos.w;

		output.Tex = float4((float2(x + w, w - y)) / 2, output.Pos.zw);

		triStream.Append(output);
	}
	triStream.RestartStrip();
}

float Contrast(float Input, float ContrastPower)
{
	bool IsAboveHalf = Input > 0.5;
	float ToRaise = saturate(2 * (IsAboveHalf ? 1 - Input : Input));
	float Output = 0.5*pow(ToRaise, ContrastPower);
	Output = IsAboveHalf ? 1 - Output : Output;
	return Output;
}

float4 PS(PARTICLE_PS_INPUT input) : SV_Target
{
	input.Tex /= input.Tex.w;

	float4 color = DiffuseTexture.Sample(linearSample_Clamp, input.UV);
	float zBuf = DepthTexture.Sample(pointSample_Clamp, input.Tex.xy).r;
	
	float SoftParticleScale = 0.5f;
	float SoftParticleContrast = 1.0f;
	float intensity = 2.0f;
	float zEpsilon = 0.0f;

	float FAR_CLIP = 1000.0f;
	float NEAR_CLIP = 0.096f;

	float ZFARMULTZNEAR = NEAR_CLIP * FAR_CLIP;
	float ZFARMINUSZNEAR = FAR_CLIP - NEAR_CLIP;
	
	float z = ZFARMULTZNEAR / (FAR_CLIP - zBuf * ZFARMINUSZNEAR);
	float zdiff = (z - input.WorldViewProj.z);

	float c = Contrast(zdiff * SoftParticleScale, SoftParticleContrast);

	if (c * zdiff <= zEpsilon)
	{
		discard;
	}

	color.a *= (half)(((c * intensity)) * input.Alpha);
	return color;
}

DepthStencilState DisableDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetDepthStencilState(DisableDepthWrites, 0);
	}
}

