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
	float4 light_position;
	float4 light_dir;
};

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
	float3 c0 		: C0;
	float3 c1 		: C1;
	float3 v3Dir	: DIR;
};

//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

float getRayleighPhase(float fCos2)
{
	return 1.0;
	return 0.75 + 0.75*fCos2; //is this even correct?
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;

	

	float g = -0.990;
	float g2 = g * g;
	

	float fCos = dot(light_dir, input.v3Dir) / length(input.v3Dir);

	float fCos2 = fCos * fCos;
	float3 color = (getRayleighPhase(fCos2) * input.c0 
	+ getMiePhase(fCos, fCos2, g, g2) * input.c1);




	return float4(color,1);

}