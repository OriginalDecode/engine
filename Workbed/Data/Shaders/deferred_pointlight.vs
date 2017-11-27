cbuffer per_frame : register (b0)
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer per_object : register(b1) 
{
	row_major float4x4 orientation;
	float range;
};

struct VS_INPUT
{
	float4 pos	: POSITION;
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float4 uv	: POSITION;
	float range : RANGE;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 scale = range;
	scale.w = 1.f;
	
	input.pos *= scale; // scale
	input.pos.w = 1.f;

	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);
	output.range = range;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.uv = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
		
	return output;
};