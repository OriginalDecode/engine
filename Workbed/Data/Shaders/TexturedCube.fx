matrix World;
matrix View;
matrix Projection;

Texture2D AlbedoTexture;

SamplerState sampleLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;

};

struct PS_INPUT
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{

	float3 color = AlbedoTexture.Sample(sampleLinear, input.UV);
	float ambient = 1.f;
	float3 ambientDiffuse = ambient * color;
	
	return float4(ambientDiffuse, 1);
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