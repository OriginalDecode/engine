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
	float4 world0 : INSTANCE0;
	float4 world1 : INSTANCE1;
	float4 world2 : INSTANCE2;
	float4 world3 : INSTANCE3;
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
	
	float4x4 world_matrices;
	world_matrices._11_12_13_14 = input.world0;
	world_matrices._21_22_23_24 = input.world1;
	world_matrices._31_32_33_34 = input.world2;
	world_matrices._41_42_43_44 = input.world3;

	float4x4 out_matrix = mul(world_matrices, View);
	out_matrix = mul(out_matrix, Projection);
	
	output.pos = mul(input.pos, out_matrix);
	//output.pos = mul(output.pos, View);
	//output.pos = mul(output.pos, Projection);
	output.uv = input.uv;
	
	output.normal = mul(input.normal, world_matrices);
	output.binorm = mul(input.binorm, world_matrices);
	output.tang  = mul(input.tang , world_matrices);
	
	output.worldpos = mul(input.pos, world_matrices);
	//output.worldpos = mul(output.worldpos, View);
	
	return output;
};