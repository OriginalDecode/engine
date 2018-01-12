cbuffer Matrices : register(b0)
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer Matrices : register(b1)
{
	row_major float4x4 orientation;
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
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float4 tex 		: TEX;
	float4 view_dir : DIR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);

	output.normal = mul(input.normal, orientation);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , orientation);

	output.uv = input.uv;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	output.worldpos = input.pos;

	output.view_dir = input.pos;

	return output;
};