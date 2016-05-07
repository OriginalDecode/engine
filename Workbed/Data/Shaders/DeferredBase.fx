#include "SharedVariables.fx"
//Wrap sampling

PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG VS(VS_INPUT_POS_NORMAL_UV_BINORMAL_TANG input)
{
	PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG output = (PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG)0;
	
	//float4 scale = float4(Scale, 1.0f);
	//input.Pos *= scale;
	
	//input.Pos.w = 1.0f;
	
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	
	
	output.UV = input.UV;
	
	
	output.Normal = mul(input.Normal, World);
	output.BiNormal = input.BiNormal;
	output.Tang  = mul(input.Tang , World);

	output.WorldPos = mul(input.Pos, World);
	
	return output;
}

GBuffer PS(PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linearSample_Wrap, input.UV) * 2 - 1;
	
	input.Normal = normalize(input.Normal);
	input.BiNormal = normalize(input.BiNormal);
	input.Tang   = normalize(input.Tang );

	float3x3 tangentSpaceMatrix = float3x3(input.Tang , input.BiNormal, input.Normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));

	GBuffer output;
	output.Albedo = AlbedoTexture.Sample(linearSample_Wrap, input.UV);
	norm.xyz += 1.f;
	norm.xyz *= 0.5f;
	output.Normal = float4(norm, 0.f);

	float depth = input.Pos.z;// / input.Pos.w;
	output.Depth = float4(depth, depth, depth, depth);
	
	return output;
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