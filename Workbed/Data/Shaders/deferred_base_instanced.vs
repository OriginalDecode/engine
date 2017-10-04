
cbuffer per_frame : register( b0 )
{
	row_major float4x4 camera_view_x_proj;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 binorm : BINORMAL;
	float4 tang : TANGENT;
	float4 world0 : INSTANCE0;
	float4 world1 : INSTANCE1;
	float4 world2 : INSTANCE2;
	float4 world3 : INSTANCE3;
	float4 data0 : DATA;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
	float2 data0 : DATA;
};

//---------------------------------
//	Deferred Base Vertex Shader
//---------------------------------
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4x4 world_matrices;
	world_matrices._11_12_13_14 = input.world0;
	world_matrices._21_22_23_24 = input.world1;
	world_matrices._31_32_33_34 = input.world2;
	world_matrices._41_42_43_44 = input.world3;

	output.data0.x = input.data0.x;
	output.data0.y = input.data0.y;

	float4x4 out_matrix = mul(world_matrices, camera_view_x_proj);
	
	output.pos = mul(input.pos, out_matrix);
	output.uv = input.uv;
	
	output.normal = mul(input.normal, world_matrices);
	output.binorm = mul(input.binorm, world_matrices);
	output.tang  = mul(input.tang , world_matrices);
	
	output.worldpos = mul(input.pos, world_matrices);
	
	return output;
};