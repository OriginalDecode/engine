#include "S_variables_pbl.fx"

float Time;
Texture2D SceneTexture;
Texture2D OffsetTexture;

shared SamplerState Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


shared SamplerState Sampler_Clamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

shared SamplerState Point
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT
{
	float3 Pos		: POSITION;
	float3 Color	: COLOR;
	float Alpha		: ALPHA;
	float Size		: SIZE;
	float Time		: TIME;
	float Rotation 	: ROTATION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Uv : TEXCOORD;
	float Size : SIZE;
	float Alpha : ALPHA;
	float Time : TIME;
	float3 Color : COLOR;
	float Rotation : ROTATION;
	float4 WorldViewProj : POS;
	float4 Tex : TEXCOORD2;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = float4(input.Pos, 1);
	output.Size = input.Size;
	output.Alpha = input.Alpha;
	output.Time = input.Time;
	//output.Pos = mul(output.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Time = input.Time;
	output.Color = input.Color;
	output.Rotation = input.Rotation;
	output.WorldViewProj = output.Pos;
	return output;
}

cbuffer cbImmutable
{
	float4 g_positions[4] =
	{
		float4(-1, 1, 0, 0),
		float4(1, 1, 0, 0),
		float4(-1, -1, 0, 0),
		float4(1, -1, 0, 0),
	};
	float2 g_texcoords[4] =
	{
		float2(0, 1),
		float2(1, 1),
		float2(0, 0),
		float2(1, 0),
	};
};


[maxvertexcount(4)]
void GS (point PS_INPUT input[1], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT output = (PS_INPUT)0;

	for (int i = 0; i < 4; ++i)
	{
		float3 rotation = 0;
		rotation.x = g_positions[i].x * cos(input[0].Rotation) - sin (input[0].Rotation) * g_positions[i].y;
		rotation.y = sin(input[0].Rotation) * g_positions[i].x + cos(input[0].Rotation) * g_positions[i].y;

		output.Pos = float4(float4(input[0].Size * rotation, 0) + input[0].Pos);
		
		
		output.Pos = mul(output.Pos, Projection);
		output.Uv = g_texcoords[i];
		output.Alpha = input[0].Alpha;
		output.Color = input[0].Color;
		output.WorldViewProj = output.Pos;		
		
		float x = output.Pos.x;
		float y = output.Pos.y;
		float w = output.Pos.w;

		output.Tex = float4((float2(x + w, w - y)) / 2, output.Pos.zw);
		
		
		triStream.Append(output);
	}

	triStream.RestartStrip();
}


float Contrast(float Input, float ContrastPower)
{
     bool IsAboveHalf = Input > 0.5 ;
     float ToRaise = saturate(2*(IsAboveHalf ? 1-Input : Input));
     float Output = 0.5*pow(ToRaise, ContrastPower); 
     Output = IsAboveHalf ? 1-Output : Output;
     return Output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	input.Tex /= input.Tex.w;
	
	float4 color = DiffuseTexture.Sample(Sampler, input.Uv);
	float4 emissive = EmissiveTexture.Sample(Sampler, input.Uv);
	emissive.rgb *= emissive.a;
	float4 currCol = float4(color.rgb * input.Color.rgb, color.a);
	currCol.rgb += emissive.rgb * emissiveMultiplier;
	
	float SoftParticleScale = 0.5f;	
	float SoftParticleContrast = 1.0f;
	float intensity = 2.0f;
	float zEpsilon = 0.0f;
	
	float FAR_CLIP = 1000.0f;
	float NEAR_CLIP = 0.096f;
	
	float ZFARMULTZNEAR = NEAR_CLIP * FAR_CLIP;
	float ZFARMINUSZNEAR = FAR_CLIP - NEAR_CLIP ;
	                      
	float zBuf = DepthTexture.Sample(Point, input.Tex.xy).r;
	
	float z = ZFARMULTZNEAR / ( FAR_CLIP - zBuf * ZFARMINUSZNEAR);
    float zdiff = (z - input.WorldViewProj.z);	
   	float c = Contrast(zdiff * SoftParticleScale, SoftParticleContrast);
	
    if( c * zdiff <= zEpsilon )
    {
		discard;
    }
	
   currCol.a *= input.Alpha;
   currCol.a *= saturate(((c * (intensity))));
   
	return currCol;
}


float4 PS_HEAT(PS_INPUT input) : SV_Target
{
	float SoftParticleScale = 0.5f;	
	float SoftParticleContrast = 1.5f;
	float intensity = 2.0f;
	float zEpsilon = 0.0f;

	input.Tex /= input.Tex.w;
	
	float4 screenPos = input.WorldViewProj;
	screenPos.xy /= screenPos.w;
	screenPos.xy /= 2;
	screenPos.xy += 0.5;
	screenPos.y = 1 - screenPos.y;
	float4 Diffuse = DiffuseTexture.Sample(Sampler, input.Uv);
	float2 halfPixel = float2(0.5f / 1920.0f,0.5f / 1080.0f);
	screenPos.y -= halfPixel.y;

	float4 offset = OffsetTexture.Sample(Sampler_Clamp, input.Uv + float2(sin(TotalTime + (input.Pos.y * 0.02)) * 0.11, 0));

	offset.xy = ((offset.xy*2.0)-1.0) * 0.03;//* (OffsetScale);
	//offset.x = ((offset.x*2.0)-1.0) * (sin(TotalTime * 1.f + (input.Pos.y * 0.01)) * 0.4f) * 0.03;//* (OffsetScale);


	float4 output = (half4)SceneTexture.Sample( Sampler_Clamp, input.Tex.xy + offset.xy); 
	output.a = 1.f;
	float FAR_CLIP = 1000.0f;
	float NEAR_CLIP = 0.096f;
	float ZFARMULTZNEAR = NEAR_CLIP * FAR_CLIP;
	float ZFARMINUSZNEAR = FAR_CLIP - NEAR_CLIP ;
	                      
	float zBuf = DepthTexture.Sample( Point, input.Tex.xy).r;
	float z = ZFARMULTZNEAR / ( FAR_CLIP - zBuf * ZFARMINUSZNEAR);
    float zdiff = (z - input.WorldViewProj.z);
    float c = Contrast(zdiff * SoftParticleScale, SoftParticleContrast);
    if( c * zdiff <= zEpsilon )
    {
        discard;
    }
    output.a = Diffuse.a;
    output.a *=  (half)(((c * intensity)* offset.a) * input.Alpha);	
    return output;
}

BlendState Blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState DisableDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWrites, 0);
	}
}

technique11 Render_HeatHaze
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		
		SetHullShader(NULL);
		SetDomainShader(NULL);

		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS_HEAT()));
		
		SetBlendState(Blend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWrites, 0);
	}
}
