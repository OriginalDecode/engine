#include "S_variables_shader.fx"

float bloomValue = 0;


SamplerState bilinearSampling
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
};

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}

float LinearToLum(float4 aColor)
{
	//return(aColor.b * 3);
	return(aColor.r*0.3+aColor.g*0.59+aColor.b*0.11);
}

float4 PS_BLOOM(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 sampleColor = DiffuseTexture.Sample(bilinearSampling, input.Tex);

	if(sampleColor.x > 1.f || sampleColor.y > 1.f || sampleColor.z > 1.f)
	{
		return float4(sampleColor.xyz, 1.f);
	}

	return float4(0.f, 0.f, 0.f, 0.f);
}

float4 PS_HDR(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 sampleColor = DiffuseTexture.Sample(bilinearSampling, input.Tex);

	return float4(sampleColor.xyz, 1.f);
}

technique11 BLOOM_DOWNSAMPLE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_BLOOM()));
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}

technique11 HDR_DOWNSAMPLE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_HDR()));
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(NULL);
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
		SetDomainShader(NULL);
		SetHullShader(NULL);
	}
}