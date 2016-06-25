//---------------------------------
//	Deferred Lightmesh Vertex Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0) //132
{
	row_major float4x4 World;
	row_major float4x4 InvertedView;
	row_major float4x4 Projection;
	float range;
	float3 filler;
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
	float4 pos	: SV_POSITION0;
	float4 uv	: POSITION;
	float range : RANGE;
};

//---------------------------------
//	Deferred Lightmesh Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4 scale = float4(range, range, range, 1.0f);
	input.pos *= scale;

	input.pos.w = 1.0f;
	output.range = range;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, InvertedView);
	output.pos = mul(output.pos, Projection);

	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.uv = float4((float2(x + w, w - y)) *0.5f, output.pos.zw);
		
	return output;
};