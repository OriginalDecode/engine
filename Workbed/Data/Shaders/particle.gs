cbuffer Matrices : register(b0)
{
	row_major float4x4 Projection;
	int index_x, index_y;
	int x,y;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
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
void main(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{
	VS_OUTPUT output = (VS_OUTPUT)0;


	float width = 1024.f / 20.f;
	float height = 512.f / 4.f;
	float2 uv;
	uv.x =  width / 1024.f;
	uv.y = height / 512.f;

	output.pos = (quadPos[0] * input[0].size.x) + input[0].pos;
	output.pos.z /= output.pos.w;
	output.pos = mul(output.pos, Projection);
	output.uv.x = uv.x + uv.x * index_x;
	output.uv.y = uv.y + uv.y * index_y;
	output.normal = float4(1,0,0,1); //quadPos[0];
	output.alpha = input[0].alpha;
	triStream.Append(output);



	output.pos = (quadPos[1] * input[0].size.x) + input[0].pos;
	output.pos.z /= output.pos.w;
	output.pos = mul(output.pos, Projection);
	output.uv.x = uv.x + uv.x * index_x;
	output.uv.y = uv.y * index_y;
	output.normal = float4(0,1,0,1);
	output.alpha = input[0].alpha;
	triStream.Append(output);




	output.pos = (quadPos[2] * input[0].size.x ) + input[0].pos;
	output.pos.z /= output.pos.w;
	output.pos = mul(output.pos, Projection);
	output.uv.x = uv.x * index_x;
	output.uv.y = uv.y + uv.y * index_y;
	output.normal = float4(0,0,1,1);
	output.alpha = input[0].alpha;
	triStream.Append(output);




	output.pos = (quadPos[3] * input[0].size.x) + input[0].pos;
	output.pos.z /= output.pos.w;
	output.pos = mul(output.pos, Projection);
	output.uv.x = uv.x * index_x;
	output.uv.y = uv.y * index_y;
	output.normal = float4(1,1,0,1);
	output.alpha = input[0].alpha;
	triStream.Append(output);


	
	triStream.RestartStrip();
};