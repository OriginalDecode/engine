matrix World;
matrix View;
matrix Projection;

float2 Scale;
float2 Position;

Texture2D DiffuseTexture;

Texture2D AlbedoTexture;
Texture2D NormalTexture;
Texture2D RoughnessTexture;
Texture2D MetalnessTexture;
Texture2D AOTexture;
Texture2D DepthTexture;

TextureCube CubeMap;

struct VS_INPUT_POS_NORM_UV
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct PS_INPUT_POS_NORM_UV
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct VS_INPUT_POS_UV
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct PS_INPUT_POS_UV
{
	float4 Pos : SV_POSITION0;
	float2 UV : TEXCOORD;
};

struct VS_INPUT_POS_COL_UV
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD;
};

struct PS_INPUT_POS_COL_UV
{
	float4 Pos : SV_POSITION0;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD;
};

struct VS_INPUT_POS_COL
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
};

struct PS_INPUT_POS_COL
{
	float4 Pos : SV_POSITION0;
	float4 Color : COLOR0;
};


struct VS_INPUT_POS_NORMAL_UV_BINORMAL_TANG
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
};

struct PS_INPUT_POS_NORMAL_UV_BINORMAL_TANG
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
	float3 WorldPos : POSITION;
};


SamplerState linearSample_Clamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState linearSample_Wrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState pointSample_Clamp
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};


