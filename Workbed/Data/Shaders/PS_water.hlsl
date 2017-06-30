//---------------------------------
//	Water Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Wrap 	: register ( s0 );
SamplerState linear_clamp 	: register ( s1 );
Texture2D RefractionTexture	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D RoughnessTexture 	: register ( t2 );
Texture2D ReflectionTexture	: register ( t3 );
Texture2D EmissiveTexture	: register ( t4 );
Texture2D OpacityTexture	: register ( t5 );
Texture2D AOTexture			: register ( t6 );
Texture2D HeightTexture		: register ( t7 );
Texture2D Displacement		: register ( t8 );
Texture2D ShininessTexture	: register ( t10 );
Texture2D DUDVTexture		: register ( t11 );
TextureCube CubeMap		 	: register ( t9 );
//---------------------------------
//	Water Base Pixel Structs
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
	float4 clip : TEXCOORD1;
};

struct DS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION0;		
	float4 clip : TEXCOORD1;
	float4 camerapos : POSITION1;
};

float3 ReflectionFresnel(const float3 substance, const float3 light_dir, const float3 half_vector, float roughness)
{
	float LdotH = dot(light_dir, half_vector);
	LdotH = saturate(LdotH);
	LdotH = 1 - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1-substance);
	fresnel = fresnel / (6 - 5 * roughness);
	fresnel = substance + fresnel;
	return fresnel;
};

//---------------------------------
//	Water Base Pixel Shader
//---------------------------------
GBuffer PS(DS_OUTPUT input) : SV_Target
{
	float2 ndc = (input.clip.xy / input.clip.w) / 2 + 0.5;
	ndc.x = (input.clip.x / input.clip.w) / 2.0h + 0.5h;
	ndc.y = (-input.clip.y / input.clip.w) / 2.0h + 0.5h;


	float3 _normal = float3(0,1,0); //NormalTexture.Sample(linear_Wrap, input.uv).rgb * 2 - 1;
	float3 normal = input.normal;
	float3 nnormal = normalize(normal);

	float3 nonParalell = float3(1.f, 0.f, 0.f);
	float3 binorm = normalize(cross(normal, nonParalell));
	float3 tang = normalize(cross(normal, binorm));
	float3x3 tangentSpaceMatrix = float3x3(tang, binorm, nnormal);
	_normal = normalize(mul(_normal.xyz, tangentSpaceMatrix));

	_normal += 1;
	_normal *= 0.5;

	float3 to_eye = normalize(input.camerapos.xyz - input.worldpos.xyz);
	float blend_value = dot(to_eye, float3(0,1,0));

	float2 reflectionUV = float2(ndc.x, -ndc.y);
	float2 refractionUV = float2(ndc.x, ndc.y);
	GBuffer output = (GBuffer)0;

	float4 reflection = ReflectionTexture.Sample(linear_Wrap, reflectionUV);
	float4 refraction = RefractionTexture.Sample(linear_Wrap, refractionUV);

	float4 distortion_texture = DUDVTexture.Sample(linear_Wrap, input.uv);
	float4 normal_texture = NormalTexture.Sample(linear_clamp, input.uv);

	float NdotL = dot(normal_texture, float4(0.2, 0.6, 0, 1));

	blend_value = pow(blend_value, 0.4);

	float4 out_color = lerp(reflection, refraction, blend_value);
	output.Albedo =  normal_texture;
	float metalness = 1.f;
	float roughness = 0.f;
	output.Normal = float4(normal_texture.rgb, metalness);
	output.Depth.y = roughness; //Roughness 
	output.Emissive = float4(1,1,1,1);
	return output;

}