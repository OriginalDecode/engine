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
Texture2D RoughnessTexture 	: register ( t1 );
Texture2D MetalnessTexture 	: register ( t2 );
Texture2D NormalTexture  	: register ( t3 );
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
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
};

//---------------------------------
//	Terrain Base Pixel Shader
//---------------------------------
GBuffer PS(VS_OUTPUT input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linear_Wrap, input.uv) * 2.f - 1.f;
	input.normal = normalize(input.normal);
	input.binorm = normalize(input.binorm);
	input.tang   = normalize(input.tang);
	
	float3x3 tangentSpaceMatrix = float3x3(input.tang, input.binorm, input.normal);
	norm = normalize(mul(input.normal, tangentSpaceMatrix));
    norm.xyz += 1.f;
	norm.xyz *= 0.5f;
	float depth = input.pos.z;
	float dist = length(float4(500.f,0.f, 500.f, 1.f) - input.worldpos);
	dist = normalize(dist);
	GBuffer output;
	
  	output.Albedo = AlbedoTexture.SampleLevel(linear_Wrap, input.uv, 1);
	output.Normal = float4(input.normal.xyz, 0.f);
	float height = input.worldpos.y;
	if(height < 0)
		height = 0;
	float4 centerColor = float4(1.0f, 1.0f, 1.0f, 1.f);
	float4 apexColor = float4(0.8f, 0.8f, 0.8f, 0.2f) ;
	
	float4 outputColor = lerp(centerColor, apexColor, height / 8);

	float4 d = length(input.pos - camPosition); 
	d = normalize(d);
 	output.Albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	
	output.Depth.r = depth;
	output.Depth.g = 1.f;
	
	return output;
}