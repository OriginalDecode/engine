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
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
	float4 clip : TEXCOORD1;
	float4 camerapos : POSITION1;
};

//---------------------------------
//	Terrain Base Vertex Shader
//---------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.clip = output.pos;

	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);
	output.camerapos = float4(CameraPos.xyz, 1);
	output.worldpos = mul(input.pos, World);
	output.uv = (input.uv) * cos(((Time / 512)* 2 - (output.worldpos.x - output.worldpos.z)))  * 32;
	return output;
};