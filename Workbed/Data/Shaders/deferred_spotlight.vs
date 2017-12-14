cbuffer per_frame : register (b0)
{
	row_major float4x4 camera_view_x_proj;
};

cbuffer per_object : register(b1) 
{
	row_major float4x4 orientation;
	float range;
    float angle;
	float intensity;
};

struct vsInput
{
	float4 pos	: POSITION;
};



struct psInput
{
	float4 pos	: SV_POSITION;
	float4 uv	: POSITION;
    float range : RANGE;
	float cosAngle : COSANGLE;
    float angle : ANGLE;
	float intensity : INTENSITY;
	float4 world : WORLD;
};

psInput main(vsInput input)
{
	psInput output = (psInput)0;
	float _angle = angle;

	float _range = range;

    float xyScale = tan(_angle ) * (_range);
	
	input.pos.x *= xyScale ; // scale
    input.pos.y *= xyScale;
    input.pos.z *= _range;
    input.pos.w = 1.f;
	
	output.pos = mul(input.pos, orientation);
	output.pos = mul(output.pos, camera_view_x_proj);
	output.cosAngle = cos(_angle); //beam width
	
	output.range = _range ;
	output.angle = _angle; //in radians
	output.intensity = intensity;
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.uv = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	output.world = input.pos;
	return output;
};