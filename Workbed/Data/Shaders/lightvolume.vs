
cbuffer Matrices : register(b0) 
{
	row_major float4x4 World;
	float2 range;
    float2 angle;
};

struct vsInput
{
	float4 pos	: POSITION;
};

struct gsInput
{
	float4 pos : SV_POSITION;
	float range : RANGE;
	float angle : ANGLE;
	float2 dummy : DUMMY;
};

gsInput main(vsInput input)
{
	gsInput output = (gsInput)0;

	output.pos = input.pos;
	output.range = range.x;

	
	return output;
}