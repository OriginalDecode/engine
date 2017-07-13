//---------------------------------
//	Line3D Vertex Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
};

//---------------------------------
//	Line3D Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 alpha : ALPHA;
	float2 size : SIZE;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
	float2 depth : DEPTH;
};


//---------------------------------
//	Line3D Vertex Shader
//---------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.depth.x = output.pos.z;

	output.pos = mul(output.pos, View);
	output.size = input.size;
	output.alpha = input.alpha;
	return output;
};