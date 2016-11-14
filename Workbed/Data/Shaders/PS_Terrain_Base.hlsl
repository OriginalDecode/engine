//---------------------------------
//	Terrain Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

cbuffer CameraPosition : register(b0)
{
	float4 camPosition;
};

SamplerState linear_Wrap 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D RoughnessTexture 	: register ( t2 );
Texture2D MetalnessTexture 	: register ( t3 );
Texture2D AOTexture			: register ( t4 );
//Texture2D EmissiveTexture	: register ( t5 );

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
	float2 uv2 : TEXCOORD2;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
};

//---------------------------------
//	Terrain Base Pixel Shader
//---------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	float3 _normal = NormalTexture.Sample(linear_Wrap, input.uv2) * 2 - 1;
	input.normal = normalize(input.normal);
	float3 nonParalell = float3(1.f, 0.f, 0.f);

	float3 binorm = cross(input.normal, nonParalell);
	float3 tang = normalize(cross(input.normal, binorm));
	float3x3 tangentSpaceMatrix = float3x3(tang, binorm, input.normal);
	_normal = normalize(mul(_normal.xyz, tangentSpaceMatrix));
	float3 _pos = float3(510,50,510); //lights world position
	float3 toLight = _pos - input.worldpos;
	float3 lightDir = normalize(toLight);
	float3 nnormal = normalize(_normal);
	float NdotL = dot(nnormal, lightDir);
	float4 lightColor = float4(saturate(albedo.rgb * NdotL), 1);// float3(NdotL, NdotL, NdotL);//saturate(albedo.rgb * NdotL);
	


	
	float ln = length(lightDir);
	float attenuation = 1.f / (1.f + 0.1f * ln + 0.01f * ln * ln);
	float _falloff = 1 - (ln / ( 3 + 0.0001));	
	float totAtt = attenuation * _falloff;
	lightColor *= totAtt;
	return lightColor;
}