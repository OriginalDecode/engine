//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

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
	GBuffer output;

	float4 opacity = OpacityTexture.Sample(linear_Wrap, input.uv);
	if(opacity.b < 1)
	discard;


	float3 _normal = NormalTexture.Sample(linear_Wrap, input.uv).rgb * 2 - 1;
	float3 nnormal = normalize(input.normal.xyz);

	float3 binorm = normalize(cross(input.normal, input.tang));
	float3 tang = normalize(cross(input.normal, binorm));
	float3x3 tangentSpaceMatrix = float3x3(input.tang, binorm, nnormal);
	_normal = normalize(mul(_normal.xyz, tangentSpaceMatrix));

	_normal += 1;
	_normal *= 0.5;

	output = (GBuffer)0;
	output.Albedo = AlbedoTexture.Sample(linear_Wrap, input.uv);
	//output.Albedo = pow(output.Albedo,  2.2);

	float metalness = MetalnessTexture.Sample(linear_Wrap, input.uv).r;

	output.Normal = float4(_normal.rgb, metalness);
	output.Depth.x = input.pos.z;
	output.Depth.y = RoughnessTexture.Sample(linear_Wrap, input.uv).r;
	output.Depth.w = 1;
	
	output.Emissive = float4(1,1,1,1);
	return output;
}
