cbuffer Matrices : register(b0)
{
	row_major float4x4 camera_view_x_proj;
};

row_major float4x4 orientation  : register(b1);

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

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);	

	output.uv2 = input.uv;
	output.uv = input.uv * 10;

	output.normal = mul(input.normal, orientation);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , orientation);

	output.worldpos = mul(input.pos, orientation);
	
	return output;
};