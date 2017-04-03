//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------

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
};

//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

static const float4 sun_position = float4(2000,700,100,0);
static const float g = -0.75;
static const float4 br = float4(5.5e-6, 13.0e-6, 22.4e-6, 0);
static const float4 bm = float4(21e-6,21e-6,21e-6,0);
static const float pi = 3.14;

float4 Br(float theta)
{
	return 3 / ( 16 * pi ) * br * ( 1 + cos(theta) * cos(theta));
}

float4 Bm(float theta)
{
	return 1 / ( 4 * pi ) * bm * (( 1 - g ) * ( 1 - g )) / ( pow( 1 + g * g - 2 * g * cos(theta), 3 / 2) ); 
}

float4 atmospheric(float theta, float s)
{
	return (Br(theta) * Bm(theta)) / (br + bm) * sun_position * (1 - exp( -(br+bm) * s ));
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	
	float4 lpos = float4(100,0,0,0);
	float4 cpos = float4(0,0,0,0);



	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;
	float4 albedo = AlbedoTexture.Sample(linear_Wrap, texCoord);

	if(depth < 1.f)
		discard;

	float4 ldir = lpos - input.worldpos;
	float s = length(input.worldpos - cpos);
	float theta = acos(dot(normalize(ldir - input.worldpos), normalize(input.worldpos - cpos) * float4(1,0,0,0)));

	float4 output = atmospheric(theta,s) * 100000;


	//height = (height - height) * 2.f - 1.f;
	
	
	if(height < 0.f)
		height = 0.f;
	
	float4 centerColor = float4(1.0f, 1.0f, 1.0f, 1.f);
	float4 apexColor = float4(0.0f, 0.0f, 0.2f, 1.f) ;
	
	float4 outputColor = lerp(centerColor, apexColor, height/12);
	outputColor.a = 1.f;
	//float4 albedo = float4(0.2f, 0.8f, 0.0f, 1.f) * input.worldpos;
	return outputColor;
}