//---------------------------------
//	Font Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------
SamplerState linear_Clamp : register ( s0 );
Texture2D DiffuseTexture : register ( t0 );

//---------------------------------
//	Font Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Font Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	return DiffuseTexture.Sample(linear_Clamp, input.uv);
};
