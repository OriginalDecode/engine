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

cbuffer createQuad
{
	float4 quadPos[4] =
	{
		float4(-1,-1,0,0),
		float4(-1,1,0,0),
		float4(1,-1,0,0),
		float4(1,1,0,0),
	};

	float2 quadUV[4] =
	{
		float2(0,1),
		float2(0,0),
		float2(1,1),
		float2(1,0),
	};
};

void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{
	Out = 0;

	for(int i = 0; i < 4; ++i)
	{
		output.Pos = (quadPos[i] * input.size.x * 2) + input[0].pos;
		output.Pos = mul(output.Pos, projection);
		output.WorldViewProj = output.pos;
		output.uv.x = quadUV[i].x;
		output.uv.y = quadUV[i].y;
		// output.Alpha = input[0].Alpha;

		// float x = output.Pos.x;
		// float y = output.Pos.y;
		// float w = output.Pos.w;

		// output.Tex = float4((float2(x + w, w - y)) / 2, output.Pos.zw);

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