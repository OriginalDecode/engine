//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Clamp : register ( s0 );
Texture2D DiffuseTexture  : register ( t0 );

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
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv);
		if(diffuse.a <= 0.f)
			discard;
	return diffuse;
}