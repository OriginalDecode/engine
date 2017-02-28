//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Clamp  : register ( s0 );
Texture2D DiffuseTexture   : register ( t0 );
Texture2D LuminanceTexture : register ( t1 );

//---------------------------------
//	Render to Texture Vertex Structs
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};


//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

static const float A = 0.22;
static const float B = 0.30;
static const float C = 0.10;
static const float D = 0.20;
static const float E = 0.01;
static const float F = 0.30;

static const float WhitePoint = 11.2;

static const float lumMin = 0.2;
static const float lumMax = 4.0;

float3 Tonemap(float3 color)
{
	return ((color*(A*color + C*B) + D*E) / (color*(A*color + B) + D*F)) - E / F;
}

float4 PS(VS_OUTPUT input) : SV_Target
{	
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv);
	float luminance = LuminanceTexture.Sample(linear_Clamp, input.uv);

	luminance = min(max(luminance.x, lumMin), lumMax);
	float exposure = 0.18;

	float3 color = (diffuse * exposure) / luminance.r;
	
	float3 curr = 2 * Tonemap(color);

	float3 white_scale = 1 / Tonemap(WhitePoint);
	float3 color2 = curr * white_scale;

	return float4(color2, diffuse.a);

}