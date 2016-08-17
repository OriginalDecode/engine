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
GBuffer PS(VS_OUTPUT input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linear_Wrap, input.uv) * 2.f - 1.f;
	input.normal = normalize(input.normal);
	input.binorm = normalize(input.binorm);
	input.tang   = normalize(input.tang);
    
	float3x3 tangentSpaceMatrix = float3x3(input.tang, input.binorm, input.normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));
    norm.xyz += 1.f;
	norm.xyz *= 0.5f;

	float depth = input.pos.z;
	/* Write splatmap stuff here */
	GBuffer output;
  	output.Albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	
	output.Normal = float4(norm.xyz,0.f);
	output.Normal.a = MetalnessTexture.Sample(linear_Wrap,input.uv).r;
	
	output.Depth.r = depth;
	output.Depth.g = RoughnessTexture.Sample(linear_Wrap, input.uv).r;
	
	return output;
}