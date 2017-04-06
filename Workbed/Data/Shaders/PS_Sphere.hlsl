//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------


cbuffer Positions : register( b0 )
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
	float4 Lin 		: LIN;
	float4 Fex 		: FEX;
};


//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

//static const float4 ESun = float4( 500, 250, 250, 0 );
//static const float g = -0.75;
//static const float4 br = float4( 5.5e-6, 13.0e-6, 22.4e-6, 1 );
//static const float bm = 21e-6;
//static const float pi = 3.14;
static const float Pi = 3.14159265;
//
//float4 rayleigh(float theta) //rayleigh
//{
//	return 3 / ( 16 * pi ) * br * ( 1 + cos(theta) * cos(theta) );
//}
//
//float4 mie(float theta) //mie
//{
//	return 1 / ( 4 * pi ) * bm * ( ( 1 - g ) * ( 1 - g ) ) / ( pow(1 + g * g - 2 * g * cos(theta), 3 / 2) );
//}
//
//float4 atmospheric(float theta, float s)
//{
//	return ( rayleigh(theta) * mie(theta) ) / ( br + bm ) * light_position * ( 1 - exp(-( br + bm ) * s) );
//}

static const float Km = 0.0025f;
static const float Kr = 0.0015f;
static const float ESun = 10.0f;
static const float3 v3InvWavelength = float3(
	1.0f / pow(0.650f, 4),
	1.0f / pow(0.570f, 4),
	1.0f / pow(0.475f, 4) );
static const float fOuterRadius = 11.25;
static const float fOuterRadius2 = fOuterRadius * fOuterRadius;
static const float fInnerRadius = 11;
static const float fInnerRadius2 = fInnerRadius * fInnerRadius;
static const float fKrESun = Kr * ESun;
static const float fKmESun = Km * ESun;
static const float fKr4PI = Kr * 4.0f * Pi;
static const float fKm4PI = Km * 4.0f * Pi;
static const float fScaleDepth = 0.25f;
static const float fInvScaleDepth = 1.0f / fScaleDepth;
static const float fScale = 1.0f / ( fOuterRadius - fInnerRadius );
static const float fScaleOverScaleDepth = fScale / fScaleDepth;

//Vec3f v3LightPos = Vec3f(0.5f, 0.5f, -0.5f);
//v3LightPos.normalize();
//Vec3f v3CameraPos = Engine->Scene->Camera->GetPosition();
//float fCameraHeight = v3CameraPos.length();
//float fCameraHeight2 = v3CameraPos.length_squared();
/*
float4 PS(VS_OUTPUT input) : SV_Target
{
	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;

	if ( depth < 1.f )
		discard;

	float4 light_dir = normalize(input.worldpos - light_position);
	float4 view_dir = normalize(input.worldpos - camera_position);
	float length_of = length(input.worldpos + camera_position);

	float n_theta = acos(dot(normalize(light_dir - input.worldpos), view_dir));

	//float theta = acos(dot(normalize(light_dir - input.worldpos), normalize(input.worldpos - camera_position) * float4(-1,0,0,0)));

	float4 output = rayleigh(n_theta) * 10000 * 5;
	output = pow(clamp(output / ( output + 1.0 ), 0, 1),  1 / 2.2);
	return output;
}
*/
// The number of sample points taken along the ray
static const int nSamples = 2;
static const float fSamples = ( float ) nSamples;
// Gravity
static const float g = -0.98f;
static const float g2 = 0.9604f;
// Shader Constants
//float4x4 WorldViewProjection;
//float3 v3CameraPos;   // The camera's current position
//float3 v3LightPos;   // The direction vector to the light source
//float3 v3InvWavelength;  // 1 / pow(wavelength, 4) for the red, green, and blue channels
//float fCameraHeight;  // The camera's current height
//float fCameraHeight2;  // fCameraHeight^2
//float fOuterRadius;   // The outer (atmosphere) radius
//float fOuterRadius2;  // fOuterRadius^2
//float fInnerRadius;   // The inner (planetary) radius
//float fInnerRadius2;  // fInnerRadius^2
//float fKrESun;	// Kr * ESun
//float fKmESun;	// Km * ESun
//float fKr4PI;	// Kr * 4 * PI
//float fKm4PI;	// Km * 4 * PI
//float fScaleDepth;   // The scale depth (the altitude at which the average atmospheric density is found)
//float fInvScaleDepth;  // 1 / fScaleDepth
//float fScale;	// 1 / (fOuterRadius - fInnerRadius)
//float fScaleOverScaleDepth; // fScale / fScaleDepth
							// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*( 0.459 + x*( 3.83 + x*( -6.80 + x*5.25 ) ) ));
}

// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ( ( 1.0 - g2 ) / ( 2.0 + g2 ) ) * ( 1.0 + fCos2 ) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2;
}

// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * ( -B - sqrt(fDet) );
}

// Returns the far intersection point of a line and a sphere
float getFarIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * ( -B + sqrt(fDet) );
}
/*
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;

	//return float4(0, 1, 0, 1);
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Pos = input.worldpos;
	float3 v3Ray = v3Pos - camera_position;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float fCameraHeight = camera_position.y;
	float fCameraHeight2 = fCameraHeight * fCameraHeight;

	float3 v3LightPos = light_position;

	float fNear = getNearIntersection(camera_position, v3Ray, fCameraHeight2, fOuterRadius2);
	// Calculate the ray's start and end positions in the atmosphere, then calculate its scattering offset
	float3 v3Start = camera_position + v3Ray * fNear;
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	float fStartDepth = exp(-fInvScaleDepth);
	float fStartOffset = fStartDepth*scale(fStartAngle);
	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	// Now loop through the sample rays
	float3 v3FrontColor = float3( 0.0, 0.0, 0.0 );
	for ( int i = 0; i < nSamples; i++ )
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * ( fInnerRadius - fHeight ));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = ( fStartOffset + fDepth*( scale(fLightAngle) - scale(fCameraAngle) ) );
		float3 v3Attenuate = exp(-fScatter * ( v3InvWavelength * fKr4PI + fKm4PI ));
		v3FrontColor += v3Attenuate * ( fDepth * fScaledLength );
		v3SamplePoint += v3SampleRay;
	}
	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	float3 c0 = v3FrontColor * ( v3InvWavelength * fKrESun );
	float3 c1 = v3FrontColor * fKmESun;
	float3 v3Direction = camera_position - v3Pos;
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fCos2 = fCos*fCos;
	float3 color = getRayleighPhase(fCos2) * c0 + getMiePhase(fCos, fCos2, g, g2) * c1;
	float4 AtmoColor = float4( color, color.b );
	float4 PlanetColor = float4( 0, 1, 0, 1 );
	return (PlanetColor + AtmoColor);
}*/

float4 PS( VS_OUTPUT input ) : SV_Target
{
	//return float4(0, 1, 0, 1);
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3CameraPos = camera_position;
	float3 v3LightPos = normalize(float4(5,5,5,1) - light_position);
	float fCameraHeight = camera_position.y;
	float fCameraHeight2 = fCameraHeight * fCameraHeight;
	float3 v3Pos = input.worldpos;

	float3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float fNear = getNearIntersection(v3CameraPos, v3Ray, fCameraHeight2, fOuterRadius2);
	// Calculate the ray's start and end positions in the atmosphere, then calculate its scattering offset
	float3 v3Start = v3CameraPos + v3Ray * fNear;
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	float fStartDepth = exp(-fInvScaleDepth);
	float fStartOffset = fStartDepth*scale(fStartAngle);
	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = (v3Start + v3SampleRay) * 0.5;
	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<2; i++)
	{
  		float fHeight = length(v3SamplePoint);
  		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
  		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
  		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
  		float3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
  		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
  		v3SamplePoint += v3SampleRay;
	}
// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	float3 c0 = v3FrontColor * (v3InvWavelength * fKrESun);

	float3 c1 = v3FrontColor * fKmESun;
	float3 v3Direction = v3CameraPos - v3Pos;
	return float4(c1,1);
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fCos2 = fCos*fCos;
	float3 color = getRayleighPhase(fCos2) * c0 + getMiePhase(fCos, fCos2, g, g2) * c1;
	float4 AtmoColor = float4(color, color.b);
	return AtmoColor;
}