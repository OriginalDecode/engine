//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------


cbuffer Positions : register(b0)
{
	float4 camera_position;
	float4 light_position;
};

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

static const float4 ESun = float4(500,250,250,0);
static const float g = -0.75;
static const float4 br = float4(5.5e-6, 13.0e-6, 22.4e-6, 1);
static const float bm = 21e-6;
static const float pi = 3.14;

float4 rayleigh(float theta) //rayleigh
{
	return 3 / ( 16 * pi ) * br * ( 1 + cos(theta) * cos(theta));
}

float4 mie(float theta) //mie
{
	return 1 / ( 4 * pi ) * bm * (( 1 - g ) * ( 1 - g )) / ( pow( 1 + g * g - 2 * g * cos(theta), 3 / 2) ); 
}

float henyeygreenstein(float theta)
{
	return ( ( 1 - g ) * ( 1 - g )) / 4 * pi * pow(( ( 1 + g ) * ( 1 + g ) ) - ( 2 * g * cos(theta)), 3/2);
}

float4 atmospheric(float theta, float s)
{
	return (rayleigh(theta) * mie(theta)) / (br + bm) * light_position * (1 - exp( -(br+bm) * s ));
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;

	if(depth < 1.f)
		discard;


	float4 light_dir = normalize( light_position - input.worldpos );
	float4 view_dir = normalize(input.worldpos - camera_position);
	float length_of = length(input.worldpos + camera_position);

	float n_theta = acos(dot(normalize(light_dir - input.worldpos), view_dir));

 	//float theta = acos(dot(normalize(light_dir - input.worldpos), normalize(input.worldpos - camera_position) * float4(-1,0,0,0)));

	float4 output = atmospheric(n_theta, length_of) * 10000 * 5 ; 
	output = pow( clamp ( output / ( output + 1.0), 0, 1),  2.2);
	return output;
}