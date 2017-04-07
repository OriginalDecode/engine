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

static const float fScaleDepth = 0.25;
// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

static const float ESun = 20;


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float PI = 3.14159265;
	int nSamples = 1;
	float fSamples = (float)nSamples;

	float red_wavelength = 0.650;
	float green_wavelength = 0.570;
	float blue_wavelength = 0.475;

	//wavelengths
	float3 v3InvWavelength = float3(1 / pow(red_wavelength, 4), 1 / pow(green_wavelength, 4), 1 / pow(blue_wavelength, 4));

	//positions
	float3 v3CameraPos = camera_position;
	float3 v3LightPos = light_pos;
	float fCameraHeight = v3CameraPos.y;
	float fCameraHeight2 = fCameraHeight*fCameraHeight;
	
	//Atmosphere radiuses
	float fOuterRadius = 10.25;
	float fOuterRadius2 = fOuterRadius*fOuterRadius;
	float fInnerRadius = 10; 
	float fInnerRadius2 = fInnerRadius*fInnerRadius;

	//Coefs
	float Kr = 0.0025f;// float3(5.5e-6, 13.0e-6, 22.4e-6);
	float fKrESun = Kr * ESun; //0.05
	float Km = 0.0010f;
	float fKmESun = Km * ESun;
	float fKr4PI = Kr * 4 * PI;
	float fKm4PI = Km * 4 * PI;

	//Depth stuff
	float fScaleDepth = 0.25;
	float fInvScaleDepth = 1 / fScaleDepth;
	float fScale = 1 / (fOuterRadius - fInnerRadius);
	float fScaleOverScaleDepth = fScale / fScaleDepth;

	float3 v3Pos = input.pos.xyz;
	float3 v3Ray = v3CameraPos - v3Pos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);
	
	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	output.c0 = v3SampleRay;//float3(fScaledLength,fScaledLength,fScaledLength);

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight2 = sqrt(length(v3SamplePoint));

		float fDepth2 = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight2));
		output.c0 = float3(fDepth2,fDepth2,fDepth2);

		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight2;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight2;
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
	
	//output.c0 = v3FrontColor * (v3InvWavelength * fKrESun); //rayleigh
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