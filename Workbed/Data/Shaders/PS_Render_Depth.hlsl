SamplerState Sampler 	: register (s0);
Texture2D DepthBuffer 	: register (t0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 worldpos : POSITION;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
	float z = input.worldpos.z;
	return float4(z,z,z,1);
}
