//---------------------------------
//	Font Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------
SamplerState linear_Clamp : register ( s0 );
Texture2D FontTexture 	 : register ( t0 );

//---------------------------------
//	Font Pixel Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Font Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 text = FontTexture.Sample(linear_Clamp, input.uv).rrrr;	
	float4 outline = FontTexture.Sample(linear_Clamp, input.uv).aaaa;
	text.rgb = float3(1,1,1);
	outline.rgb = float3(0,0,0);
	if(text.a > 0.4)
	{
		outline.rgb += text.rgb;
	}
	float4 output = outline;
	return output;
};
