#include "constants.hlsl"
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
	float4 sprite =  DiffuseTexture.Sample(linear_Clamp, input.uv);
	if(sprite.a <= 0)
		sprite.a = 1;
	
	return sprite;
	float miZ = 0.9;
	float maZ = 1;
	float newX = (sprite.x - miZ) / (maZ - miZ);	
	sprite.x = newX;
	sprite.y = newX;
	sprite.z = newX;
	return sprite;
};
