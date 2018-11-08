cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer Matrices : register(b1)
{
	row_major float4x4 orientation;
	row_major float4x4 camera;
	float4 dummy;
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
	output.pos.w = 1;
	output.normal = float3(0,1,0);
	output.tang = float3(1,0,0);
	output.binorm = float3(0,0,1);
	output.uv = input.uv;

	output.camera_pos = camera._m30_m31_m32_m33;
	output.worldpos = mul(input.pos, orientation);


	return output;
};