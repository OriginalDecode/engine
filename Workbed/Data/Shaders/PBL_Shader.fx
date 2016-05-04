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

struct POINT_LIGHT
{
	float4 Color;
	float3 Position;
	float  Range;
};

POINT_LIGHT PointLight[4];

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
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION0;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tang : TANGENT;
	float3 WorldPos : POSITION;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Normal = mul(input.Normal, World);
	output.UV = input.UV;
	
	output.Tang = mul(input.Tang, World);
	output.WorldPos = mul(input.Pos, World);

	return output;
}

static const float ambient = 0.42f;

float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return attenuation;
}

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 1.f - (someDistance / (someRange + 0.1f));
	return fallOff;
}

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return totalAttenuation;
}

float2 hash(float2 p) { p = float2(dot(p, float2(127.1, 311.7)), dot(p, float2(269.5, 183.3))); return frac(sin(p)*18.5453); }

        // return distance, and cell id
        float2 voronoi(in float2 x)
        {
            float2 n = floor(x);
            float2 f = frac(x);

            float3 m = 8.0;
            for (int j = -1; j <= 1; j++)
                for (int i = -1; i <= 1; i++)
                {
                    float2  g = float2(float(i), float(j));
                    float2  o = hash(n + g);
                    //vec2  r = g - f + o;
                    float2  r = g - f + (0.5 + 0.5 *o);
                    float d = dot(r, r);
                    if (d<m.x)
                        m = float3(d, o);
                }

            return float2(sqrt(m.x), m.y + m.z);
        }

float4 PS(PS_INPUT input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(sampleLinear, input.UV);
	float3 binormal = normalize(cross(input.Normal, input.Tang));
	float3 normal = NormalTexture.Sample(sampleLinear, input.UV);
	float4 roughness = RoughnessTexture.Sample(sampleLinear, input.UV);
	float4 metalness = SubstanceTexture.Sample(sampleLinear, input.UV);
	
	float3x3 tangentSpaceMatrix = float3x3(input.Tang, binormal, input.Normal);

	float4 diffuse = ambient * albedo;
	normal = normalize(normal);
	input.Tang = normalize(input.Tang);
	normal = normalize(mul(normal, tangentSpaceMatrix));
	//return float4(input.Normal.xyz, 1);
	float4 finalColor = 0.0f;
	//return DirectionalLight[0].Direction;
	
	//for(int i = 0; i < 1; i++)
	//{
	//	float lambert = dot(-DirectionalLight[i].Direction, normal);
	//	finalColor += saturate(lambert * DirectionalLight[i].Color);
	//}
	
	for(int i = 0; i < 4; i++)
	{
		float3 lightDirection = PointLight[i].Position - input.WorldPos;
		float distance = length(lightDirection);
		lightDirection = normalize(lightDirection);
		
		float lambert = dot(lightDirection, normal);
		finalColor += saturate(lambert * PointLight[i].Color) * CalculateTotalAttenuation(distance,PointLight[i].Range);
		finalColor = saturate(finalColor * 2);
	}
	//float2 b = voronoi(14.0 + 6.0 * input.UV);

	
	finalColor.a = 1;
	return float4(diffuse.rgb + (albedo.rgb * finalColor.rgb), 1);
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
