//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Wrap : register ( s0 );
Texture2D AlbedoTexture  : register ( t0 );
Texture2D NormalTexture  : register ( t1 );

//---------------------------------
//	Deferred Base Pixel Structs
//---------------------------------
struct GBuffer
{
	float4 Albedo;
	float4 Normal;
	float4 Depth;
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
GBuffer PS(VS_OUTPUT input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linear_Wrap, input.uv) * 2.f - 1.f;
	
	input.normal = normalize(input.normal);
	input.binorm = normalize(input.binorm);
	input.tang   = normalize(input.tang);
    
	float3x3 tangentSpaceMatrix = float3x3(input.tang, input.binorm, input.normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));
    
	GBuffer output;
  	output.Albedo = AlbedoTexture.Sample(linear_Wrap, input.uv) * 1.f;//ambientMultiplier;
	norm.xyz += 1.f;
	norm.xyz *= 0.5f;
	output.Normal = float4(norm, 0.f);
    
	float depth = input.pos.z;
	output.Depth = float4(depth, depth, depth, depth);
	//float d = input.pos.z;
	//float4 output = float4(AlbedoTexture.Sample(linear_Wrap, input.uv));
	//float4 output = float4(d,d,d,1);
	return output;
}