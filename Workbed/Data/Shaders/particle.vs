cbuffer Matrices : register(b0)
{
	row_major float4x4 view_projection;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 alpha : ALPHA;
	float2 size : SIZE;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Line3D Vertex Shader
//---------------------------------

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, 1);
	// output.pos = mul(output.pos, view_projection);
	output.size = input.size;
	output.alpha = input.alpha;
	
	return output;
};