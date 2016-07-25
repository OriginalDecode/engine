//---------------------------------
//	Particle Geometry Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 InvertedView;
	row_major float4x4 Projection;
};


cbuffer createQuad
{
	float4 quadPos[4] =
	{
		float4(-1,-1,0,0),
		float4(-1,1,0,0),
		float4(1,-1,0,0),
		float4(1,1,0,0),
	};

	float2 quadUV[4] =
	{
		float2(0,1),
		float2(0,0),
		float2(1,1),
		float2(1,0),
	};
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
	output.pos = mul(output.pos, InvertedView);
	output.pos = mul(output.pos, Projection);
	output.color = input.color;
	
	return output;
};