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
void GS (point PARTICLE_PS_INPUT input[1], inout TriangleStream<PARTICLE_PS_INPUT> triStream)
{
	PARTICLE_PS_INPUT output = (PARTICLE_PS_INPUT)0;
	
	for(int i = 0; i < 4; ++i)
	{
		output.Pos = (quadPos[i] * input[0].Size) + input[0].Pos;
		output.Pos = mul(output.Pos, Projection);
		
		output.UV.x = quadUV[i].x;
		output.UV.y = quadUV[i].y;
		
		output.Alpha = input[0].Alpha;
		triStream.Append(output);
	}	
	triStream.RestartStrip();
}

float4 PS(PARTICLE_PS_INPUT input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(linearSample_Clamp, input.UV);
	color.a *= input.Alpha;

	return color;	
}

BlendState AlphaBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState DisableDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0,VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		//SetBlendState(AlphaBlend, float4(0.f,0.f,0.f,0.f),0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWrites, 0);
	}
}

