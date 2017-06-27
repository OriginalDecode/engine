cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
};

struct HS_OUTPUT
{
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;	
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct DS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION0;	
};

[domain("quad")]
DS_OUTPUT DS(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
	float3 vertex_pos;
	DS_OUTPUT output = (DS_OUTPUT)0;

	vertex_pos = uvwCoord.x * patch[0].pos + uvwCoord.y * patch[2].pos;

	output.pos = mul(float4(vertex_pos,1), World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.normal = patch[0].normal;
	output.uv = patch[0].uv;
	output.binorm = patch[0].binorm;
	output.tang = patch[0].tang;
	output.worldpos = patch[0].worldpos;

	return output;
}