//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------


//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------


cbuffer Positions : register(b0)
{
	float4 camera_position;
	float4 camera_dir;
	float4 light_dir;
	float4 light_pos;
	float inner_radius;
	float outer_radius;
	float camera_magnitude;
	float camera_magnitude2;
};

SamplerState linear_Wrap : register ( s0 );
Texture2D DepthTexture : register ( t7 );

//---------------------------------
//	Deferred Base Pixel Structs
//---------------------------------


struct VS_OUTPUT
{
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float4 tex 		: TEX;
};


//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).r;

	float4 output = input.worldpos;

	return output;
}