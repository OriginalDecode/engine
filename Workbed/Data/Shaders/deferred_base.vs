
cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

row_major float4x4 orientation : register (b1);

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 binorm : BINORMAL;
	float4 tang : TANGENT;
	float4 worldpos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 binorm : BINORMAL;
	float4 tang : TANGENT;
	float4 worldpos : POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	input.pos.w = 1.f;

	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);	
	output.uv = input.uv;

	output.normal = mul(input.normal, orientation);
	output.binorm = mul(input.binorm, orientation);
	output.tang  = mul(input.tang , orientation);
	output.worldpos = mul(input.pos, orientation);

	return output;
};