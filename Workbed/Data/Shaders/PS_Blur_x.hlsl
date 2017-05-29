cbuffer Blur : register(b0)
{
	float4 TexelWidth;
}

Texture2D SSAOTexture	: register ( t0 );
SamplerState SSAOSampler 	: register ( s0 );

struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

static const int kernelSize = 13;
float PixelKernel[kernelSize] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	+1,
	+2,
	+3,
	+4,
	+5,
	+6
};
static const float BlurWeights[kernelSize] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = { 0, 0, 0, 1 };
	float3 uv = { 0, input.uv.y, 0};

	for (int i = 0; i < kernelSize; ++i)
	{
		uv.x = input.uv.x + (PixelKernel[i] * TexelWidth.x);
		float4 ssao_tex = SSAOTexture.Sample(SSAOSampler, uv);
		color += ssao_tex * BlurWeights[i];
	}

	return color;
}
