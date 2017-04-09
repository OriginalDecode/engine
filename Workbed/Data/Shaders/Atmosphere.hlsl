static const int TRANSMITTANCE_TEXTURE_WIDTH = 256;
static const int TRANSMITTANCE_TEXTURE_HEIGHT = 64;
static const int SCATTERING_TEXTURE_R_SIZE = 32;
static const int SCATTERING_TEXTURE_MU_SIZE = 128;
static const int SCATTERING_TEXTURE_MU_S_SIZE = 32;
static const int SCATTERING_TEXTURE_NU_SIZE = 8;
static const int IRRADIANCE_TEXTURE_WIDTH = 64;
static const int IRRADIANCE_TEXTURE_HEIGHT = 16;
static const int kLambdaMin = 360;
static const int kLambdaMax = 830;
static const float kSunAngularRadius = 0.00935f / 2.f;
static const float kSunSolidAngle = 3.1415926f * kSunAngularRadius * kSunAngularRadius;
static const float kUnitLengthInMeters = 1000.f;
static const float kPi = 3.141593;
static const float kConstantSolarIrradiance = 1.5;
static const float kBottomRadius = 6360000.0;
static const float kTopRadius = 6420000.0;
static const float kRayleigh = 1.24062e-6;
static const float kRayleighScaleHeight = 8000.0;
static const float kMieScaleHeight = 1200.0;
static const float kMieAngstromAlpha = 0.0;
static const float kMieAngstromBeta = 5.328e-3;
static const float kMieSingleScatteringAlbedo = 0.9;
static const float kMiePhaseFunctionG = 0.8;
static const float kGroundAlbedo = 0.1;
static const float kMaxSunZenithAngle = 102.0 / 180.0 * kPi;
static const float kLambdaR = 680.0;
static const float kLambdaG = 550.0;
static const float kLambdaB = 440.0;

cbuffer DefaultBuffer : register( b0 )
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;

};

cbuffer AtmosphereBuffer : register( b1 )
{
	float3 sky_spec_rad_to_lum;
	float3 sun_spec_rad_to_lum;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
};

struct VS_OUTPUT
{


};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);

	output.uv = input.uv;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	output.worldpos = input.pos;


	return output;
};

void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{

};


float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).r;


	return float4(1,1,1,1);
};