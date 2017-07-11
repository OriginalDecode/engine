//---------------------------------
//	Terrain Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

cbuffer clipvalue : register(b0)
{
	float4 clip_value;
	float4 camera_position;
};

SamplerState linear_Wrap 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D RoughnessTexture 	: register ( t2 );
Texture2D MetalnessTexture 	: register ( t3 );
Texture2D EmissiveTexture	: register ( t4 );
Texture2D OpacityTexture	: register ( t5 );
Texture2D AOTexture			: register ( t6 );
Texture2D HeightTexture		: register ( t7 );
Texture2D Displacement		: register ( t8 );
Texture2D LightmapTexture	: register ( t9 );
Texture2D ShininessTexture	: register ( t10 );
//---------------------------------
//	Terrain Base Pixel Structs
//---------------------------------
struct GBuffer
{
	float4 Albedo;
	float4 Normal;
	float4 Depth;
	float4 Emissive;
};

struct VS_OUTPUT
{
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float2 uv2 		: TEXCOORD2;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
};

//---------------------------------
//	Terrain Base Pixel Shader
//---------------------------------
GBuffer PS(VS_OUTPUT input) : SV_Target
{
	float value = dot(input.worldpos, clip_value);
	if(value < 0)
		discard;
	float3 _normal = NormalTexture.Sample(linear_Wrap, input.uv2).rgb * 2 - 1;
	float3 nnormal = normalize(input.normal.xyz);

	float3 nonParalell = float3(1.f, 0.f, 0.f);
	float3 binorm = normalize(cross(input.normal, nonParalell));
	float3 tang = normalize(cross(input.normal, binorm));
	float3x3 tangentSpaceMatrix = float3x3(tang, binorm, nnormal);
	_normal = normalize(mul(_normal.xyz, tangentSpaceMatrix));

	_normal += 1;
	_normal *= 0.5;


	GBuffer output;
	output = (GBuffer)0;
	output.Albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	output.Normal = float4(_normal.rgb, 0);//MetalnessTexture.Sample(linear_Wrap, input.uv).r);
	output.Depth.x = input.pos.z;
	output.Depth.y = RoughnessTexture.Sample(linear_Wrap, input.uv / 32).r;
	output.Depth.w = 1;
	output.Emissive = float4(1,1,1,1);
	return output;

}