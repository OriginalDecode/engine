//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState linear_Clamp  : register ( s0 );
Texture2D DiffuseTexture   : register ( t0 );
Texture2D LuminanceTexture : register ( t1 );
Texture2D AverageLumTexture : register ( t2 );

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

float4 Tonemap(float4 color)
{
	return ((color*(A*color + C*B) + D*E) / (color*(A*color + B) + D*F)) - E / F;
}

float4 PS(VS_OUTPUT input) : SV_Target
{	
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv);

	float4 LumPixel = LuminanceTexture.Sample(linear_Clamp, input.uv);
	float luminance = AverageLumTexture.Sample(linear_Clamp, input.uv).r;

	luminance = min(max(luminance, lumMin), lumMax);
	float exposure = (1.03 - (2 / (2 + log10(luminance + 1 ) ) ) ) ;

	float4 color1 = (diffuse * (0.5)) / luminance;
	float4 curr = Tonemap( 2 * diffuse);
	float4 white_scale = 1 / Tonemap(WhitePoint);
	float4 color = curr * white_scale;

	//float4 retColor = pow(color, 1/2.2);

	float4 output = saturate(pow( color, 1 / 2.2 ));

	output.rgb = (output.rgb - 0.5) * (1.0 + 0.3) + 0.5;
	return saturate(output  * color1);
}

/*
float4 PS(VS_OUTPUT input) : SV_Target
{	
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv);
	float luminance = AverageLumTexture.Sample(linear_Clamp, input.uv).r;
	luminance = min(max(luminance, lumMin), lumMax);
	float exposure = 0.18;
	float4 texColor = diffuse * (exposure) / (luminance); 
	float4 curr = 2 * Tonemap(texColor);
	float4 white_scale = 1 / Tonemap(WhitePoint);
	float4 color = curr * white_scale;
	return float4(color.rgb,diffuse.a);
}
*/