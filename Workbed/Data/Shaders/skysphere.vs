//_________________________________
//	Atmospheric Scattering Vertex Shaders
//_________________________________
//_________________________________
//	Constant Buffers
//_________________________________
cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
};

cbuffer Positions : register(b1)
{
	float4 camera_position;
	float4 camera_dir;
	float4 light_dir;
	float4 light_pos;
	float inner_radius;
	float outer_radius;
	float camera_magnitude;
	float camera_magnitude2;

};

//_________________________________
//	Vertex Structs
//_________________________________
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
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float4 tex 		: TEX;
};

//_________________________________
//	Atmospheric Scattering Vertex Shader
//_________________________________

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);

	output.uv = input.uv;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	output.worldpos = input.pos;


	return output;
};