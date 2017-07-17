//---------------------------------
//	Water Vertex Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	float3 CameraPos;
	float Time;
};

//---------------------------------
//	Terrain Base Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
};

struct VS_OUTPUT
{
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;
	float4 clip : TEXCOORD1;
};

//---------------------------------
//	Terrain Base Vertex Shader
//---------------------------------
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = input.pos;
	output.uv = input.uv;
	output.clip = output.pos;

	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);

	output.worldpos = mul(input.pos, World);
	return output;
};