//---------------------------------
//	Base Vertex Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0)
{
	row_major matrix World;
	row_major matrix InvertedView;
	row_major matrix Projection;
};


//---------------------------------
//	Base Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 Pos 		: POSITION;
	float3 Color 	: COLOR;
};

struct VS_OUTPUT
{
	float4 Pos 		: SV_POSITION0;
	float3 Color	: COLOR;
};

//---------------------------------
//	Base Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, InvertedView);
	output.Pos = mul(output.Pos, Projection);
	
	output.Color = input.Color;
		
	return output;
}