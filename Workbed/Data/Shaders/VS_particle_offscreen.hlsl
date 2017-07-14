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
	float4 eyePos : POSITION0;
	float4x4 projection : MATRIX0;
};

//---------------------------------
//	Line3D Vertex Shader
//---------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.eyePos = output.pos;
	output.size = input.size;
	output.alpha = input.alpha;
	return output;
};