//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Clamp : register ( s0 );
Texture2D DiffuseTexture  : register ( t0 );
Texture2D DepthTexture	  	: register ( t1 );
Texture2D LightTexture		: register ( t2 );
//---------------------------------
//	Render to Texture Vertex Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{	
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv) ;
	float4 light = LightTexture.Sample(linear_Clamp, input.uv);
	light.a = 1;
	return diffuse * light;
}