//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Wrap 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D RoughnessTexture 	: register ( t1 );
Texture2D MetalnessTexture 	: register ( t2 );
Texture2D NormalTexture  	: register ( t3 );
Texture2D AOTexture			: register ( t4 );
//Texture2D EmissiveTexture	: register ( t5 );

//---------------------------------
//	Deferred Base Pixel Structs
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
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
};

//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	float3 _normal = NormalTexture.Sample(linear_Wrap, input.uv) * 2 - 1;

	float3 binorm = normalize(cross(input.normal, input.tang));
	float3x3 tangentSpaceMatrix = float3x3(input.tang, binorm, input.normal);
	_normal = normalize(mul(_normal.xyz, tangentSpaceMatrix));
	return float4(_normal.rgb, 1);

	float3 _pos = float3(25,0,29); //lights world position
	float3 toLight = _pos - input.worldpos;
	float3 lightDir = normalize(toLight);
	float3 nnormal = normalize(_normal);
	float NdotL = dot(nnormal, lightDir);
	float3 lightColor = saturate(float3(1,1,1) * NdotL);
	return float4(NdotL, NdotL, NdotL, 1);
}