matrix World;
matrix Projection;
matrix View;

float2 SpritePosition;
Texture2D AlbedoTexture;

SamplerState sampleLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	//float4 color : COLOR0;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION0;
	//float4 color : COLOR0;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos.x += SpritePosition.x;
	output.pos.y += SpritePosition.y;	
	output.pos = mul(output.pos, Projection);
	//output.color = input.color;	
	output.UV = input.UV;
	return output;
};

float4 PS(PS_INPUT input) : SV_Target
{
	//return float4(1,1,0,1);
	float4 color = AlbedoTexture.Sample(sampleLinear,input.UV).aaaa;	
	return color;
};

BlendState AlphaBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

RasterizerState RS
{
	CullMode = NONE;
};


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetRasterizerState(RS);
		//SetBlendState(AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	
	}
}
