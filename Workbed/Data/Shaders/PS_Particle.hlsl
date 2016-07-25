//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

//---------------------------------
//	Line3D Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

//---------------------------------
//	Line3D Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 position = input.pos;
	
	return input.color * position;
};
