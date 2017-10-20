cbuffer per_frame : register (b0)
{
    row_major float4x4 invview;
	row_major float4x4 view;
    row_major float4x4 proj;
};

cbuffer per_object : register (b1)
{
    row_major float4x4 orientation;
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
    float4x4 new_orientation = view;
    new_orientation ._41_42_43_44 = pos;

    output.pos = mul(input.pos, new_orientation);
    output.pos = mul(output.pos, invview);
    output.pos = mul(output.pos, proj);

    output.uv = input.uv;   

    return output;
};