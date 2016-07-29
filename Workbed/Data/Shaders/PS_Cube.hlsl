//---------------------------------
//	Base Pixel Shader
//---------------------------------
//---------------------------------
//	Base Pixel Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
	float4 Color 	: COLOR;
};

//---------------------------------
//Base Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	return float4(0.5f, 0.5f,0.5f, 1.f);
};
 