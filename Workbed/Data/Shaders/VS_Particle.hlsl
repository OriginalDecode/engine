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
	row_major float4x4 Projection;
};

//---------------------------------
//	Line3D Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


//---------------------------------
//	Line3D Vertex Shader
//---------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.color = input.color;
	
	return output;
};