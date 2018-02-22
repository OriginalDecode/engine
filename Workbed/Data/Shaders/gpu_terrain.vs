cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer Matrices : register(b1)
{
	row_major float4x4 orientation;
	row_major float4x4 camera;
	float Time;
	float3 padding;
};

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
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;
	float4 camera_pos : POSITION2;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = input.pos;
	output.uv = input.uv;

	output.normal = mul(input.normal, orientation);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , orientation);

	output.camera_pos = camera._m30_m31_m32_m33;
	output.worldpos = input.pos;//mul(input.pos, orientation);

	return output;
};