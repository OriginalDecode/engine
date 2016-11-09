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
	float4 albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	float3 norm = NormalTexture.Sample(linear_Wrap, input.uv) * 2 - 1;

	input.normal = normalize(input.normal);
	input.binorm = normalize(input.binorm);
	input.tang   = normalize(input.tang);
    
	float3x3 tangentSpaceMatrix = float3x3(input.tang, input.binorm, input.normal);
	float3 _normal = normalize(mul(norm.xyz, tangentSpaceMatrix));

	/* Write splatmap stuff here */

	float depth = normalize(input.worldpos.z);
	GBuffer output;
	output.Albedo = float4(1,1,1,1);
	output.Normal = float4(_normal.xyz, 1);
	output.Depth = float4(depth, depth, depth, depth);

	//output.Depth.x = depth;
	//output.Depth.g = 0.f;//RoughnessTexture.Sample(linear_Wrap, input.uv).r;
	
	//output.Normal.a = 0.f;// MetalnessTexture.Sample(linear_Wrap, input.uv).r;
	return output;
}