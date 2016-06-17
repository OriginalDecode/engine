//---------------------------------
//	Base Pixel Shader
//---------------------------------
//---------------------------------
//	Base Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 Pos 		: SV_POSITION;
	float3 Color	: COLOR;
};

//---------------------------------
//	Base Pixel Shader
//---------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 output = float4(input.Color.rgb, 1);
	return output;
}