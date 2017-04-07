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
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION;
	float4 tex : TEX;
};

//_________________________________
//	Atmospheric Scattering Vertex Shader
//_________________________________


// The scale equation calculated by Vernier's Graphical Analysis
float Scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

// Calculates the Mie phase function
float GetMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float GetRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2; //is this even correct?
}

// Returns the near intersection point of a line and a sphere
float GetNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}

float GetFarIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B + sqrt(fDet));
}

static const float3 ESun = float3(20,20,20);

VS_OUTPUT VS(VS_INPUT input)
{
	float PI = 3.14159265;
	int nSamples = 2;
	float fSamples = (float)nSamples;

	//wavelengths
	float3 v3InvWavelength = float3(1/pow(0,4),1/pow(0,4),1/pow(0,4));

	//positions
	float3 v3CameraPos = camera_position;
	float3 v3LightPos = light_pos;
	float fCameraHeight = v3CameraPos.y;
	float fCameraHeight2 = fCameraHeight*fCameraHeight;
	
	//Atmosphere radiuses
	float fOuterRadius = 2.25;
	float fOuterRadius2 = fOuterRadius*fOuterRadius;
	float fInnerRadius = 2; 
	float fInnerRadius2 = fInnerRadius*fInnerRadius;

	//Coefs
	float3 Kr = float3(0,0,0);
	float fKrESun = Kr * ESun;
	float Km = 0;
	float fKmESun = Km * ESun;
	float fKr4PI = Kr * 4 * PI;
	float fKmPI = Km * 4 * PI;

	//Depth stuff
	float fScaleDepth = 0.25;
	float fInvScaleDepth = 1 / fScaleDepth;
	float fScale = 1 / (fOuterRadius - fInnerRadius);
	float fScaleOverScaleDepth = fScale / fScaleDepth;



	




	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.pos.z = output.pos.w;
	output.uv = input.uv;
	
	output.normal = mul(input.normal, World);
	output.binorm = input.binorm;
	output.tang  = mul(input.tang , World);
	
	float x = output.pos.x;
	float y = output.pos.y;
	float w = output.pos.w;

	output.tex = float4((float2(x + w, w - y)) * 0.5f, output.pos.zw);
	
	output.worldpos = input.pos;

	return output;
};