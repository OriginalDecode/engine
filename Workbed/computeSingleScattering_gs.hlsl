
struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

#define VERTEX_COUNT 3

static const float4 quadPos[VERTEX_COUNT] =
{
	float4(-1,-1,0,0),
	float4(-1,1,0,0),
	float4(1,-1,0,0),
};

static const float2 quadUV[VERTEX_COUNT] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
};

[maxvertexcount(VERTEX_COUNT)]
VS_OUTPUT main(point VS_OUTPUT input, inout TriangleStream<VS_OUTPUT> triStream) 
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	for(int i = 0; i < VERTEX_COUNT; ++i)
	{
		output.pos = (quadPos[i] * input[0].size.x * 2) + input[0].pos;
		output.pos.z = output.pos.z / output.pos.w; 
		output.pos = mul(output.pos, Projection);

		output.uv.x = quadUV[i].x;
		output.uv.y = quadUV[i].y;

		triStream.Append(output);
	}
	triStream.RestartStrip();
}