cbuffer per_frame : register (b0)
{
    row_major float4x4 camera_view_x_proj;
};

cbuffer per_object : register (b1)
{
    row_major float4x4 orientation;
};

cbuffer per_light : register (b2)
{
    row_major float4x4 camera_orientation; //get the rotation from this matrix    
};


struct vsInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct psInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

psInput main(vsInput input)
{
    psInput output = (psInput)0;
    float4 pos = orientation._41_42_43_44;
    float4x4 new_orientation = camera_orientation;
    new_orientation ._41_42_43_44 = pos;

    output.pos = mul(input.pos, new_orientation);
    output.pos = mul(output.pos, camera_view_x_proj);

    output.uv = input.uv;   

    return output;
};