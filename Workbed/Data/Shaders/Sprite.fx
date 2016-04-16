matrix World;
matrix Projection;
matrix View;

Texture2D AlbedoTexture;

SamplerState sampleLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION0;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	
	output.UV = input.UV;
	return output;
};

float4 PS(PS_INPUT input) : SV_Target
{
	float4 color = AlbedoTexture.Sample(sampleLinear,input.UV);
	return float4(1,1,0,1);
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
