
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
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4x4 world_matrices;
	world_matrices._11_12_13_14 = input.world0;
	world_matrices._21_22_23_24 = input.world1;
	world_matrices._31_32_33_34 = input.world2;
	world_matrices._41_42_43_44 = input.world3;

	output.normal = mul(input.normal, world_matrices);
	output.binorm = mul(input.binorm, world_matrices);
	output.tang  = mul(input.tang , world_matrices);
	output.worldpos = mul(input.pos, world_matrices);

	output.uv = input.uv;
	float4 dv = HeightTexture.SampleLevel(sampler0, output.uv, 0);
	output.displaced = dv;
	float df = 0.30*dv.x + 0.59*dv.y + 0.11*dv.z;

	float4 displacement = float4(input.normal.xyz * df * 0.0, 0) + input.pos; 
	float4x4 out_matrix = mul(world_matrices, camera_view_x_proj);
	output.pos = mul(displacement, out_matrix);

	return output;
};