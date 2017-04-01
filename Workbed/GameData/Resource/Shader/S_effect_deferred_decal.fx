#include "S_variables_pbl.fx"
#include "S_light_point.fx"

float3 DecalDirection;

struct VS_DECAL
{
	float4 Position : POSITION;
};

struct PS_DECAL
{
	float4 Position : SV_POSITION;
	float4 Tex : TEXCOORD;
	float4 ClipPosition : POSITION;
};


float3 GetDecalNormal(float4 aWorldPosition)
{
	float3 ddxWp = ddx(aWorldPosition);
	float3 ddyWp = ddy(aWorldPosition);

	return normalize(cross(ddyWp, ddxWp));
}

float3 CalculateWorldSpaceNormal(float4 aWorldPosition, float2 aTexCoord)
{
	float3 ddxWp = ddx(aWorldPosition);
	float3 ddyWp = ddy(aWorldPosition);

	float3 normal = normalize(cross(ddyWp, ddxWp));
	float3 binormal = normalize(ddxWp);
	float3 tangent = normalize(ddyWp);

	float3x3 tangentToView;
	tangentToView[0] = mul(tangent, View);
	tangentToView[1] = mul(binormal, View);
	tangentToView[2] = mul(normal, View);


	float3 finalNormal = DecalNormalTexture.Sample(linearSamplingClamp, aTexCoord);
	finalNormal = mul(finalNormal, tangentToView);

	return finalNormal;	
}

PS_DECAL VS(VS_DECAL input)
{
	PS_DECAL output = (PS_DECAL)0;

	input.Position.xyz *= float3(4.f, 4.f, 8.f);
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	float x = output.Position.x;
	float y = output.Position.y;
	float w = output.Position.w;

	output.Tex = float4((float2(x + w, w - y)) / 2, output.Position.zw);

	output.ClipPosition = output.Position;

	return output;
}

float4 CalcWorldPosition(float2 aTexCoord)
{
	float depth = DepthTexture.Sample(pointSample, aTexCoord).x;

	float x = aTexCoord.x * 2.f - 1.f;
	float y = (1.f - aTexCoord.y) * 2.f - 1.f;
	float z = depth;

	float4 worldPosition = float4(x, y, z, 1.f);

	worldPosition = mul(worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, NotInvertedView);

	return worldPosition;
}

float4 CalcObjectPosition(float4 aWorldPosition)
{
	float4 objectPosition = mul(float4(aWorldPosition.xyz, 1), WorldInverted);
	objectPosition.xyz /= float3(4.f, 4.f, 8.f);
	clip(0.5 - abs(objectPosition.xyz));

	return objectPosition;
}

/*
struct GBuffer
{
	float4 Albedo;   //
	float4 Normal;   //Metalness in W
	float4 Emissive; //AO in W
	float2 Depth;	 //Roughness in Y
};
*/

GBuffer PS(PS_DECAL input) : SV_Target
{
	input.Tex /= input.Tex.w;

	GBuffer oldBuffer;
	oldBuffer.Albedo = AlbedoTexture.Sample(pointSample, input.Tex.xy);
	oldBuffer.Normal = NormalTexture.Sample(pointSample, input.Tex.xy);
	oldBuffer.Emissive = EmissiveTexture.Sample(pointSample, input.Tex.xy);
	oldBuffer.Depth = DepthTexture.Sample(pointSample, input.Tex.xy).xy;


	float4 worldPosition = CalcWorldPosition(input.Tex.xy);
	float4 objectPosition = CalcObjectPosition(worldPosition);


	float3 normal = GetDecalNormal(worldPosition);
	float angle = acos(dot(normal, DecalDirection));
	//float difference = 0.785398163f - angle;
	float difference = 1.4f - angle;
	clip(difference);

	float2 decalTexCoord = objectPosition.xy + 0.5f;

	float4 sourceAlbedo = DiffuseTexture.Sample(linearSamplingClamp, decalTexCoord);
	float4 sourceMetalness = MetalnessTexture.Sample(linearSamplingClamp, decalTexCoord);
	float4 sourceRoughness = RoughnessTexture.Sample(linearSamplingClamp, decalTexCoord);
	//sourceMetalness.x = 1.f;
	//sourceRoughness.x = 0.1f;

	float blendAlpha = sourceAlbedo.w;
	blendAlpha *= Alpha;

	float3 finalRGB = (sourceAlbedo.xyz * blendAlpha) + (oldBuffer.Albedo.xyz * (1.f - blendAlpha));
	float finalAlpha = 0.f + oldBuffer.Albedo.w;

	float finalMetalness = oldBuffer.Normal.w + sourceMetalness.x * blendAlpha;
	finalMetalness = (sourceMetalness.x * blendAlpha) + (oldBuffer.Normal.w * (1.f - blendAlpha));

	float finalRoughness = oldBuffer.Depth.y + sourceRoughness.x * blendAlpha;
	finalRoughness = (sourceRoughness.x * blendAlpha) + (oldBuffer.Depth.y * (1.f - blendAlpha));


	float3 destNormal = oldBuffer.Normal.xyz;
	destNormal.xyz *= 2.0f;
	destNormal.xyz -= 1.f;
	float3 sourceNormal = CalculateWorldSpaceNormal(worldPosition, decalTexCoord);
	float3 finalNormal = float3(0, 0, 0);

	
	finalNormal = (destNormal * sourceNormal) * 0.5f;
	finalNormal = lerp(destNormal, sourceNormal, blendAlpha);
	finalNormal = normalize(finalNormal);
	
	finalNormal.xyz += 1.f;
	finalNormal.xyz *= 0.5f;

	GBuffer output;
	output.Albedo = float4(finalRGB, finalAlpha);
	//output.Normal = oldBuffer.Normal;
	output.Normal.xyz = finalNormal;
	output.Normal.w = finalMetalness;
	output.Emissive = oldBuffer.Emissive;
	output.Depth = oldBuffer.Depth;
	output.Depth.y = finalRoughness;

	return output;
}

float3 BlendRGB(float4 aSource, float4 aDest)
{
	float blendAlpha = aSource.w;
	blendAlpha *= Alpha;

	float3 finalRGB = (aSource.xyz * blendAlpha) + (aDest.xyz * (1.f - blendAlpha));
	return finalRGB;
}

float4 GetFinalAlbedo(float2 aDecalTex, float2 aScreenTex)
{
	float4 source = DiffuseTexture.Sample(linearSample, aDecalTex);

	float4 dest = AlbedoTexture.Sample(pointSample, aScreenTex);
	float blendAlpha = source.w;
	blendAlpha *= Alpha;

	float3 finalRGB = BlendRGB(source, dest);
	float finalAlpha = (0.f + dest.w) * blendAlpha;

	return float4(finalRGB, finalAlpha);
}

float4 GetFinalEmissive(float2 aDecalTex, float2 aScreenTex)
{
	float4 source = DecalEmissiveTexture.Sample(linearSample, aDecalTex);
	float4 dest = EmissiveTexture.Sample(pointSample, aScreenTex);
	
	float3 finalRGB = BlendRGB(source, dest);
	float finalAlpha = dest.w;

	return float4(finalRGB, finalAlpha);
}


struct DecalPixelOutput
{
	float4 Albedo;
	float4 Emissive;
};

float4 PS_NO_PBL(PS_DECAL input) : SV_Target
{
	input.Tex /= input.Tex.w;

	float4 worldPosition = CalcWorldPosition(input.Tex.xy);
	float4 objectPosition = CalcObjectPosition(worldPosition);


	float3 normal = GetDecalNormal(worldPosition);
	float angle = acos(dot(normal, DecalDirection));
	float difference = 1.4f - angle;
	clip(difference);

	float2 decalTexCoord = objectPosition.xy + 0.5f;

	float4 output = GetFinalAlbedo(decalTexCoord, input.Tex.xy);
	float4 emissive = DecalEmissiveTexture.Sample(linearSample, decalTexCoord);
	emissive.xyz *= Alpha;
	output.xyz += emissive.xyz * decalEmissiveMultiplier;
	return output;
/*
	DecalPixelOutput output;
	output.Albedo = GetFinalAlbedo(decalTexCoord, input.Tex.xy);
	output.Emissive = GetFinalEmissive(decalTexCoord, input.Tex.xy);

	return output;

	*/
}


BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

technique11 Render
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(NoBlend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);

	}
}

technique11 Render_NO_PBL
{
	pass P0
	{
		SetDomainShader(NULL);
		SetHullShader(NULL);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_NO_PBL()));
		SetBlendState(NoBlend, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}