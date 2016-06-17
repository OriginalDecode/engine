//---------------------------------
//	Base Vertex Shaders
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


//---------------------------------
//	Base Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 Pos 		: POSITION;
	float3 Normal 	: NORMAL;
	float2 UV 		: TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang 	: TANGENT;
};

struct VS_OUTPUT
{
	float4 Pos 		: SV_POSITION0;
	float3 Normal 	: NORMAL;
	float2 UV 		: TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang 	: TANGENT;
	float3 WorldPos : POSITION;
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
    
	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	
	output.Tang = mul(input.Tang, World);
	output.WorldPos = mul(input.Pos, World);

	return output;
}