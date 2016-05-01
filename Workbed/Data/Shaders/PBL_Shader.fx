matrix World;
matrix View;
matrix Projection;

Texture2D AlbedoTexture;
Texture2D NormalTexture;
Texture2D RoughnessTexture;
Texture2D SubstanceTexture;
Texture2D AOTexture;

struct DIRECTIONAL_LIGHT
{
	float4 Color;
	float4 Direction;
};

DIRECTIONAL_LIGHT DirectionalLight[1];

SamplerState sampleLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	return output;
}

static const float ambient = 0.42f;

float4 PS(PS_INPUT input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(sampleLinear, input.UV);
	float4 normal = AlbedoTexture.Sample(sampleLinear, input.UV);
	float4 roughness = RoughnessTexture.Sample(sampleLinear, input.UV);
	float4 metalness = SubstanceTexture.Sample(sampleLinear, input.UV);
	
	
	float4 diffuse = ambient * albedo;
	
	//return float4(input.Normal.xyz, 1);
	float4 finalColor = 0.0f;
	//return DirectionalLight[0].Direction;
	for(int i = 0; i < 1; i++)
	{
		float lambert = dot(input.Normal, -DirectionalLight[i].Direction);
		finalColor += saturate(lambert * DirectionalLight[i].Color);
	}
	
	return float4(diffuse + (albedo * finalColor));
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
