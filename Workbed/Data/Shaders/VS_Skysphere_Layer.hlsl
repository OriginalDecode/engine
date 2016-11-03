//---------------------------------
//	Deferred Base Vertex Shaders
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
//	Deferred Base Vertex Structs
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
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
	float4 tex : TEX;
};

//---------------------------------
//	Deferred Base Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, InvertedView);
	output.pos = mul(output.pos, Projection);
	output.pos.z = output.pos.w;
	output.uv = input.uv;
	
	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	
	output.worldpos = mul(input.pos, World);
	
	return output;
};