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
static const float4 rayleigh_scatter_coef = float4(5.5e-6, 13.0e-6, 22.4e-6, 1);
static const float mie_scatter_coef = 21e-6;
static const float pi = 3.14;



float rayleigh_phase(float theta)
{
    return ( 3 / (16 * pi) * (1+  ( 0.5 * (1 - cos(2*theta)) )) );
}

float4 rayleigh(float theta) //rayleigh
{
	return 3 / ( 16 * pi ) * rayleigh_scatter_coef * ( 1 + cos(theta) * cos(theta));
}

float4 mie(float theta) //mie
{
	return 1 / ( 4 * pi ) * mie_scatter_coef * (( 1 - g ) * ( 1 - g )) / ( pow( 1 + g * g - 2 * g * cos(theta), 3 / 2) );
}

float henyeygreenstein(float theta, float g_in)
{
    float neg_gg = ( 1 - g_in ) * ( 1 - g_in );
    float pos_gg = ( 1 + g_in ) * ( 1 + g_in );
    float g2 = 2 * g_in;

	return ( neg_gg ) / 4 * pi * pow(( pos_gg ) - ( g2 * cos(theta)), 3/2);
}

float4 atmospheric(float theta, float s)
{
	return (rayleigh(theta) * mie(theta)) / ( rayleigh_scatter_coef + mie_scatter_coef ) * light_position * (1 - exp( -( rayleigh_scatter_coef + mie_scatter_coef ) * s ));
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
	
	float4 light_dir = normalize( input.worldpos - light_position );
	float4 view_dir = normalize(input.worldpos - camera_position);
	float length_of = length(input.worldpos + camera_position);

	float n_theta = acos(dot(normalize(light_dir - input.worldpos), view_dir));

 	//float theta = acos(dot(normalize(light_dir - input.worldpos), normalize(input.worldpos - camera_position) * float4(-1,0,0,0)));

	float4 output = rayleigh(n_theta) * 100000 * 5;// atmospheric(n_theta, length_of) * 1000 * 5;
	//output = pow( clamp ( output / ( output + 1.0), 0, 1),  1 /2.2);
	return output;
}