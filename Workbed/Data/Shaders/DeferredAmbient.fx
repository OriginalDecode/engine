#include "SharedVariables.fx"


PS_INPUT_POS_UV VS(VS_INPUT_POS_UV input)
{
	PS_INPUT_POS_UV output = (PS_INPUT_POS_UV)0;

	output.UV = input.UV;
	output.Pos = input.Pos;

	return output;
}

float4 PS(PS_INPUT_POS_UV input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(pointSample_Clamp, input.UV);
	float4 normal = NormalTexture.Sample(pointSample_Clamp, input.UV);
	normal.xyz *= 2.0f;
	normal.xyz -= 1.f;
	//float4 cubemap = CubeMap.SampleLevel(pointSample, normal, 7);

	//return saturate(albedo * cubemap);
	return albedo * 0.42f;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}