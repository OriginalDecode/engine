cbuffer Matrices : register(b0)
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer _cb1 : register ( b1 )
{
	row_major float4x4 orientation;
	row_major float4x4 camera;
}
struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float2 uv2 : TEXCOORD2;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
};

SamplerState sampler0 : register(s0);
Texture2D HeightTexture : register (t7);
VS_OUTPUT main(VS_INPUT input)
{
	float4 camera_position = camera._m30_m31_m32_m33;
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4 dv = HeightTexture.SampleLevel(sampler0, output.uv, 0);
	float df = 0.30*dv.x + 0.59*dv.y + 0.11*dv.z;
	float4 displacement = float4(input.normal.xyz * df * 0.5, 0) + input.pos; 

	output.pos = mul(displacement, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);	
	output.worldpos = mul(displacement, orientation);


	output.uv2 = input.uv;
	output.uv = input.uv * 10;

	output.normal = mul(input.normal, orientation);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , orientation);





	return output;
};