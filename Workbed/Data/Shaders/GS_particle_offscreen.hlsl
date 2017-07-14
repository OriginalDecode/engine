//---------------------------------
//	Particle Geometry Shaders
//---------------------------------
//---------------------------------
//	Constant Buffers
//---------------------------------
cbuffer Matrices : register(b0)
{
	row_major float4x4 Projection;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
	float2 depth : DEPTH;
	float4 eyePos : POSITION0;
	float4x4 projection : MATRIX0;
};

static const float4 quadPos[4] =
{
	float4(-1,-1,0,0),
	float4(-1,1,0,0),
	float4(1,-1,0,0),
	float4(1,1,0,0),
};

static const float2 quadUV[4] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};

[maxvertexcount(4)]
void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	for(int i = 0; i < 4; ++i)
	{
		output.eyePos = input[0].eyePos;
		output.pos = (quadPos[i] * input[0].size.x * 2) + input[0].pos;

		output.pos = mul(output.pos, Projection);

		output.pos.z /= output.pos.w;

		output.uv.x = quadUV[i].x;
		output.uv.y = quadUV[i].y;
		output.depth = input[0].depth;
		output.alpha = input[0].alpha;
		triStream.Append(output);
	}
	triStream.RestartStrip();
};


// //---------------------------------
// //	Line3D Vertex Structs
// //---------------------------------
// struct VS_INPUT
// {
// 	float4 pos : POSITION;
// 	float4 color : COLOR;
// };

// struct VS_OUTPUT
// {
// 	float4 pos : SV_POSITION;
// 	float4 color : COLOR;
// };


// //---------------------------------
// //	Line3D Vertex Shader
// //---------------------------------

// VS_OUTPUT VS(VS_INPUT input)
// {
// 	VS_OUTPUT output = (VS_OUTPUT)0;
// 	output.pos = mul(input.pos, World);
// 	output.pos = mul(output.pos, InvertedView);
// 	output.pos = mul(output.pos, Projection);
// 	output.color = input.color;
	
// 	return output;
// };