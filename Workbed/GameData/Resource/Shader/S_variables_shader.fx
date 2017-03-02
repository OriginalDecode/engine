Matrix World;
Matrix WorldInverted;
Matrix View;
Matrix Projection;
Matrix ViewProjection;
Matrix InvertedProjection;
Matrix NotInvertedView;
Matrix ShadowMVP;

Texture2D DiffuseTexture;
Texture2D FogOfWarTexture;
Texture2D ShadowDepth;

Texture2D AlbedoTexture;
Texture2D NormalTexture;
Texture2D DecalNormalTexture;
Texture2D DecalEmissiveTexture;
Texture2D RoughnessTexture;
Texture2D MetalnessTexture;
Texture2D AOTexture;
Texture2D EmissiveTexture;
Texture2D DepthTexture;
Texture2D RandomTexture;

TextureCube CubeMap;

float3 Scale;
float4 Color;
float Alpha;

BlendState BlendDefault
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
	SrcBlend[0] = ONE;
	DestBlend[0] = ZERO;
	BlendOp[0] = ADD;
	SrcBlendAlpha[0] = ONE;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState DepthDefault
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS;
	StencilEnable = FALSE;
	StencilReadMask = 0xFF;
	StencilWriteMask = 0xFF;
};

struct GBuffer
{
	float4 Albedo;   //Roughness in W
	float4 Normal;   //Metalness in W
	float4 Emissive; //AO in W
	float2 Depth;	 //Roughness in Y
};

struct VS_LIGHTMESH
{
	float4 Position : POSITION;
};

struct PS_LIGHTMESH
{
	float4 Position : SV_POSITION;
	float4 Tex : POSITION;
};

struct PS_LIGHTMESH_TEXTUREPROJECTION
{
	float4 Position : SV_POSITION;
	float4 Tex : POSITION;
	float4 ViewPosition : TEXCOORD0;
};

struct VS_INPUT_POS_NORM_TEX
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_NORM_TEX
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_INPUT_1UVSET
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
};

struct VS_INPUT_1UVSET_INSTANCED
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 world0 : myWorld0;
	float4 world1 : myWorld1;
	float4 world2 : myWorld2;
	float4 world3 : myWorld3;
	float3 scale : myScale;
};

struct PS_INPUT_1UVSET
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};

struct PS_INPUT_1UVSET_WATER
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
	float fVertexDistanceFactor : TEXCOORD1;
	float4x4 WaterWorld : TEXCOORD2;
};

struct VS_INPUT_1UVSET_COLOR
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
};

struct VS_INPUT_1UVSET_COLOR_INSTANCED
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float4 world0 : myWorld0;
	float4 world1 : myWorld1;
	float4 world2 : myWorld2;
	float4 world3 : myWorld3;
	float3 scale : myScale;
};


struct PS_INPUT_1UVSET_COLOR
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float4 WorldPosition : POSITION;
};

struct VS_INPUT_2UVSET
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
};

struct VS_INPUT_2UVSET_INSTANCED
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 world0 : myWorld0;
	float4 world1 : myWorld1;
	float4 world2 : myWorld2;
	float4 world3 : myWorld3;
	float3 scale : myScale;
};

struct PS_INPUT_2UVSET
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};

struct VS_INPUT_2UVSET_COLOR
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
};

struct VS_INPUT_2UVSET_COLOR_INSTANCED
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float4 world0 : myWorld0;
	float4 world1 : myWorld1;
	float4 world2 : myWorld2;
	float4 world3 : myWorld3;
	float3 scale : myScale;
};

struct PS_INPUT_2UVSET_COLOR
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
	float2 Tex2 : TEXCOORD1;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float4 WorldPosition : POSITION;
};

struct VS_INPUT_POS_NORM_TEX_BI_TANG
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
};

struct VS_INPUT_POS_NORM_TEX_BI_TANG_INSTANCED
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 world0 : myWorld0;
	float4 world1 : myWorld1;
	float4 world2 : myWorld2;
	float4 world3 : myWorld3;
	float3 scale : myScale;
};

struct PS_INPUT_POS_NORM_TEX_BI_TANG
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};

struct PS_INPUT_POS_NORM_TEX_BI_TANG_SHADOW
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
	float4 DirectionalLightCoordinates : POSITION1;
};

struct VS_INPUT_POS_TEX
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_TEX
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};



struct VS_INPUT_POS_COL
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT_POS_COL
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};



struct VS_INPUT_POS_NORM_COL
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

struct PS_INPUT_POS_NORM_COL
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};


struct VS_INPUT_POS_COL_UV
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_COL_UV
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};


float3 Lambert(float3 aLightDirction, float3 aNormal, float3 aColor)
{
	float lambert = dot(aLightDirction, aNormal);
	float3 lightColor = saturate(lambert * aColor);

	return lightColor;
}

float Attenuation(float3 aLightVec, float aRange)
{
	float distance = length(aLightVec);
	//return 1 - (distance / aRange);

	float attenuation = 1.f;// / (1.f + 0.1f * distance + 0.01f * distance * distance);
	float fallOff = 0.9f - (distance / (aRange + 0.00001f));
	return saturate(attenuation * fallOff);
}

float AngularAttenuation(float3 aLightVec, float3 aLightDirection, float aLightCone)
{
	float angularAttenuation = dot(-aLightVec, aLightDirection);
	angularAttenuation -= 1.f - aLightCone;
	angularAttenuation *= 1.f / aLightCone;

	return angularAttenuation;
}