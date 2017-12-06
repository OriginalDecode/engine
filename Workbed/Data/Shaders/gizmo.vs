struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
};

cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer per_object : register ( b1 )
{
    row_major float4x4 orientation;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.pos = mul(input.pos, orientation);
    output.pos = mul(output.pos, camera_view_x_proj);

    output.normal = input.normal;
    return output;
};