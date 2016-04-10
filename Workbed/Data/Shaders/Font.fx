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
	float2 Pos : POSITION0;
	float2 Size : POSITION1;
	float2 uvStart : TEXCOORD0;
	float2 uvEnd : TEXCOORD1;
};

struct GS_INPUT
{
	float2 Pos : POSITION0;
	float2 Size : POSITION1;
	float2 uvStart : TEXCOORD0;
	float2 uvEnd : TEXCOORD1;
};


struct PS_INPUT
{
	float4 Pos : SV_Position;
	float2 uv : TEXCOORD0;
};


VS_INPUT main(VS_INPUT input)
{
	VS_INPUT output;
	output.pos = mul(float4(input.pos, 0.f, 1.f), World).xy * InvResolution * 2.f - float2(1.f, 1.f);
	output.pos.y = -output.pos.y;
	output.size = input.size * InvResolution * 2.f;
	output.size.y = -output.size.y;
	output.uvStart = input.uvStart;
	output.uvEnd = input.uvEnd;

	return output;
}

[maxvertexcount(4)]
void main(point GS_INPUT input[1], inout TriangleStream stream)
{
	float2 pos = input[0].pos;
	float2 size = input[0].size;
	float2 uvBegin = input[0].uvStart;
	float2 uvEnd = input[0].uvEnd;

	GS_OUTPUT output0, output1, output2, output3;
	output0.pos = float4(pos, 1.0f, 1.0f);
	output0.uv = uvBegin;

	output1.pos = float4(pos.x + size.x, pos.y, 1.0f, 1.0f);
	output1.uv = float2(uvEnd.x, uvBegin.y);

	output2.pos = float4(pos.x, pos.y + size.y, 1.0f, 1.0f);
	output2.uv = float2(uvBegin.x, uvEnd.y);

	output3.pos = float4(pos + size, 1.0f, 1.0f);
	output3.uv = uvEnd;

	stream.Append(output0);
	stream.Append(output1);
	stream.Append(output2);
	stream.Append(output3);
	stream.RestartStrip();
}

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
	return float4(1,1,1,1);
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
