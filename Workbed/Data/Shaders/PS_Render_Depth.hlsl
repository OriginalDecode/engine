 SamplerState Sampler 	: register (s0);
Texture2D DepthBuffer 	: register (t0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 worldpos : POSITION;
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
	float4 depth = DepthBuffer.Sample(Sampler, input.uv);
	
	float3 uv;
	uv.y = input.uv.y;
	uv.x = input.uv.x;

	float4 output = float4(depth.r,depth.r,depth.r,depth.r);
	
	float TexelWidth = 1/32;
	float TexelHeight = 1/32;


	for (int i = 0; i < kernelSize; ++i)
	{
		uv.x = input.uv.x + (PixelKernel[i] * TexelWidth);
		output *= BlurWeights[i];
	}

	for (int i = 0; i < kernelSize; ++i)
	{
		uv.y = input.uv.y + (PixelKernel[i] * TexelHeight);
		output *= BlurWeights[i];
	}
	
	return output;
}
