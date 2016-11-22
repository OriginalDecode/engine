//---------------------------------
//	Deferred Lightmesh Vertex Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0) 
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	float4 range;
};
//---------------------------------
//	Deferred Lightmesh Vertex Structs
//---------------------------------

struct VS_INPUT
{
	float4 pos	: POSITION;
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float4 uv	: POSITION;
	float4 range : RANGE;
};

//---------------------------------
//	Deferred Lightmesh Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float r = 110;
	float4 scale = r;
	scale.w = 1.f;
	
	input.pos *= scale; // scale
	input.pos.w = 1.f;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.range = r;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.uv = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
		
	return output;
};