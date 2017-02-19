//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Wrap : register ( s0 );
Texture2D AlbedoTexture  : register ( t0 );
Texture2D DepthTexture   : register ( t1 );

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
	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;
	if(depth < 1.f)
		discard;

	if(height < 0.f)
		height = 0.f;
	
	float4 centerColor = float4(1.0f, 1.0f, 1.0f, 1.f);
	float4 apexColor = float4(0.0f, 0.0f, 0.2f, 1.f) ;
	
	float4 outputColor = lerp(centerColor, apexColor, height/8);

	float4 albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	return outputColor;
}