cbuffer Matrices : register(b0) 
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	float2 range;
    float2 angle;
};

struct VS_INPUT
{
	float4 pos	: POSITION;
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float4 uv	: POSITION;
	float2 cosAngle : COSANGLE;
    float2 range : RANGE;
};

struct gsInput
{
	float4 pos : SV_POSITION;
	float2 range : RANGE;
	float2 dummy : DUMMY;
};

gsInput main(VS_INPUT input)
{
	gsInput output = (gsInput)0;
	output.pos = input.pos;
	output.range.x = range.x;
	return output;

    /*float xyScale = tan(angle.x) * range.x;
	
	input.pos.x *= xyScale; // scale
    input.pos.y *= xyScale;
    input.pos.z *= range.x;
    input.pos.w = 1.f;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.cosAngle.x = cos(angle.x / 2);
	output.range.x = range.x;
	output.range.y = angle.x;


	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.uv = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);*/
		
	return output;
};