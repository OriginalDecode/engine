//_________________________________
//	Atmospheric Scattering Vertex Shaders
//_________________________________
//_________________________________
//	Constant Buffers
//_________________________________
cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	
};

cbuffer Positions : register(b1)
{
	float4 camera_position;
	float4 light_dir;
	float4 light_pos;
	float camera_magnitude2;
	float camera_magnitude;
	float pad;
	float pad2;
};

//_________________________________
//	Vertex Structs
//_________________________________
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
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float4 tex 		: TEX;
	float3 c0 		: C0;
	float3 c1 		: C1;
	float3 v3Dir	: DIR;
};

//_________________________________
//	Atmospheric Scattering Vertex Shader
//_________________________________

// The scale equation calculated by Vernier's Graphical Analysis


static const float ESun = 20;
static const float red_wavelength = 0.650;
static const float green_wavelength = 0.570;
static const float blue_wavelength = 0.475;
static const float3 v3InvWavelength = float3(
		1 / pow(red_wavelength, 4), 
		1 / pow(green_wavelength, 4), 
		1 / pow(blue_wavelength, 4));
static const float PI = 3.14159265;
static const float Kr = 0.0025f;// float3(5.5e-6, 13.0e-6, 22.4e-6);
static const float fKrESun = Kr * ESun; //0.05
static const float Km = 0.0010f;
static const float fKmESun = Km * ESun;
static const float fKr4PI = Kr * 4 * PI;
static const float fKm4PI = Km * 4 * PI;
//Atmosphere radiuses
static const float fOuterRadius = 10.25;
static const float fOuterRadius2 = fOuterRadius*fOuterRadius;
static const float fInnerRadius = 10; 
static const float fInnerRadius2 = fInnerRadius*fInnerRadius;	
//Depth stuff
static const float fScaleDepth = 0.25;
static const float fInvScaleDepth = 1 / fScaleDepth;
static const float fScale = 1 / (fOuterRadius - fInnerRadius);
static const float fScaleOverScaleDepth = fScale / fScaleDepth;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*( 0.459 + x*( 3.83 + x*( -6.80 + x*5.25 ) ) ));
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	//positions
	float3 v3CameraPos = camera_position.xyz;
	float3 v3LightPos = light_dir;
	float fCameraHeight = camera_magnitude;// > fOuterRadius ?  fOuterRadius : camera_magnitude ;
	float fCameraHeight2 = camera_magnitude2;

	float3 v3Pos = input.pos.xyz;
	float3 v3Ray = (v3Pos - v3CameraPos);
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);
	

	int nSamples = 4;
	float fSamples = (float)nSamples;
	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = (v3Ray * fSampleLength);
	float3 v3SamplePoint = (v3Start + v3SampleRay) * 0.5;
	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight2 = length(v3SamplePoint);

		float fDepth2 = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight2));

		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight2;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / length(v3Pos);
		float fScatter = (fStartOffset + fDepth2*(scale(fLightAngle) - scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth2 * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}


	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.pos.z = output.pos.w;
	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);
	
	output.c0 = v3FrontColor * (v3InvWavelength * fKrESun); //rayleigh
	output.c1 = v3FrontColor * fKmESun; //mie
	

	output.uv = input.uv;
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	
	output.worldpos = input.pos;

	output.v3Dir = v3CameraPos - v3Pos;

	return output;
};