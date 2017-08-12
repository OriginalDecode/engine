
cbuffer Matrices : register(b0) 
{
	row_major float4x4 World;
};

struct vsInput
{
	float3 pos	: POSITION;
	float angle : ANGLE;
	float range : RANGE;
	float3 direction : DIRECTION;
};

struct gsInput
{
	float4 pos : SV_POSITION;
	float range : RANGE;
	float angle : ANGLE;
	float cosAngle : ANGLE;
	float3 direction : DIRECTION;
};

gsInput main(vsInput input)
{
	gsInput output = (gsInput)0;

	output.pos = float4(input.pos,1);
	output.angle = input.angle;
	output.cosAngle = cos(input.angle);
	output.range = input.range;
	output.direction = input.direction;

	return output;
}