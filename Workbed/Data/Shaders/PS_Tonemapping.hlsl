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
	float luminance = AverageLumTexture.Sample(linear_Clamp, input.uv);

	//luminance = min(max(luminance, lumMin), lumMax);
	float exposure = 0.18;//(1.03 - (2 / (2 + log10(luminance + 1 ) ) ) ) ;

	float4 color1 = (LumPixel * (exposure * 2)) / luminance;
	float4 curr = Tonemap( 2 * diffuse);
	float4 white_scale = 1 / Tonemap(WhitePoint);
	float4 color = curr * white_scale;

	//float4 retColor = pow(color, 1/2.2);

	float4 output = saturate(pow( color, 1 / 2.2 ));

	//output = diffuse * ( color1 / ( 1 + color1  ));
	//output = diffuse * ( color1 / ( 1 + color1 / (lumMax * lumMax))) / ( 1 + color1);
	output.rgb = (output.rgb - 0.5) * (1.0 + 0.3) + 0.5;
	return saturate(diffuse  * color1);
}
/*
	float4 diffuse = DiffuseTexture.Sample(linear_Clamp, input.uv);
	float luminance = LuminanceTexture.Sample(linear_Clamp, input.uv);

	luminance = min(max(luminance.x, lumMin), lumMax);
	float exposure = 0.18;

	float3 color = (diffuse * exposure) / luminance.r;
	
	float3 curr = 2 * Tonemap(color);

	float3 white_scale = 1 / Tonemap(WhitePoint);
	float3 color2 = curr * white_scale;

	return float4(color2, diffuse.a);
*/