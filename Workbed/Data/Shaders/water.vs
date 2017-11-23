cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer Matrices : register(b1)
{
	row_major float4x4 orientation;
	float3 CameraPos;
	float Time;
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
	float4 clip : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = input.pos;
	output.uv = input.uv;
	output.clip = output.pos;

	output.normal = mul(input.normal, orientation);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , orientation);

	output.worldpos = mul(input.pos, orientation);
	return output;
};