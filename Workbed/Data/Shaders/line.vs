
cbuffer per_frame : register (b0)
{
	row_major float4x4 camera_view_x_proj;
};

row_major float4x4 orientation : register(b1);

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	input.pos.w = 1;

	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);

	output.color = input.color;
	
	return output;
}; 