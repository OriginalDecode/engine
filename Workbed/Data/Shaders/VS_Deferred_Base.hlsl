//---------------------------------
//	Deferred Base Vertex Shaders
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
//	Deferred Base Vertex Structs
//---------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 world0 : model_instance0;
	float4 world1 : model_instance1;
	float4 world2 : model_instance2;
	float4 world3 : model_instance3;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
};

//---------------------------------
//	Deferred Base Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.uv = input.uv;
	
	output.normal = mul(input.normal, World);
	output.binorm = mul(input.binorm, World);
	output.tang  = mul(input.tang , World);
	
	output.worldpos = mul(input.pos, World);
	output.worldpos = mul(output.worldpos, View);
	
	return output;
};