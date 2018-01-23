cbuffer Matrices : register(b0)
{
	row_major float4x4 view;
	row_major float4x4 proj;
	float2 Position;
	float2 Scale;
}; //is it this one?

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
}; //40 byte

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
}; //40 byte

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	

	output.pos = mul(input.pos, view);
	output.pos = input.pos;
	output.pos.x += Position.x;
	output.pos.y += Position.y; 	
	output.pos = mul(input.pos, proj);
	output.color = input.color;	
	output.uv = input.uv;
	
	return output;
};