#include "S_variables_pbl.fx"
#include "S_light_spot.fx"


PS_LIGHTMESH_TEXTUREPROJECTION VS(VS_LIGHTMESH input)
{
	PS_LIGHTMESH_TEXTUREPROJECTION output = (PS_LIGHTMESH_TEXTUREPROJECTION)0;

	float xyScale = tan(SpotLights[0].Angle) * SpotLights[0].Range;

	input.Position.x *= xyScale;
	input.Position.y *= xyScale;
	input.Position.z *= SpotLights[0].Range;
	input.Position.w = 1.0f;

	Matrix mat = mul(World, ViewProjection);
	output.Position = mul(input.Position, mat);

	float x = output.Position.x;
	float y = output.Position.y;
	float w = output.Position.w;


	output.Tex = float4((float2(x + w, w - y)) / 2, output.Position.zw);
	float4 normal = normalize(World._m20_m21_m22_m23);


	input.Position.w = 1.f;
	output.ViewPosition = mul(input.Position, World);
	output.ViewPosition = mul(output.ViewPosition, View);
	output.ViewPosition = mul(output.ViewPosition, Projection);

	return output;
}


float4 PS(PS_LIGHTMESH_TEXTUREPROJECTION input) : SV_Target
{
	//return float4(0.f, 0.f, 0.f, 0.f);

	input.Tex /= input.Tex.w;
	DeferredPixelData pixelData = CalculateDeferredPixelData(input.Tex.xy);

	float3 lightToPixel = normalize(pixelData.myWorldPosition.xyz - SpotLights[0].Position);
		float spotFactor = dot(lightToPixel, normalize(SpotLights[0].Direction));

	//if(spotFactor > SpotLights[0].CosAngle)
	//{
	//	float3 toLight = SpotLights[0].Position - pixelData.myWorldPosition;
	//	float3 lightDir = normalize(toLight);

	//	

	//	float linearAttenuation = Attenuation(toLight, SpotLights[0].Range);
	//	//float angularAttenuation = AngularAttenuation(lightDir, SpotLights[0].Direction.xyz, SpotLights[0].Angle);
	//	float angularAttenuation = (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - SpotLights[0].CosAngle));
	//	float attenuation = linearAttenuation * angularAttenuation;
	//	
	//	//return float4(angularAttenuation, angularAttenuation, angularAttenuation, angularAttenuation);
	//	//return float4(linearAttenuation, linearAttenuation, linearAttenuation, linearAttenuation);
	//	//return float4(attenuation, attenuation, attenuation, attenuation);
	//	//return float4(SpotLights[0].Color.xyz *linearAttenuation, linearAttenuation);

	//	float3 viewPos = cameraPosition;
	//	float3 toEye = normalize(viewPos - pixelData.myWorldPosition.xyz);
	//	float3 halfVec = normalize(lightDir + toEye.xyz);
	//	float NdotL = saturate(dot(pixelData.myNormal, toLight));
	//	float HdotN = saturate(dot(halfVec, pixelData.myNormal));

	//	float3 F = saturate(Fresnel(pixelData.mySubstance, lightDir, halfVec));
	//	float D = saturate(D_GGX(HdotN, (pixelData.myRoughness + 1) / 2));
	//	float V = saturate(V_SchlickForGGX((pixelData.myRoughness + 1) / 2, dot(pixelData.myNormal, toEye), NdotL));


	//	float intensity = SpotLights[0].Color.w;

	//	float3 lightColor = SpotLights[0].Color.xyz * intensity;
	//	float3 directSpec = (((D * V * F) / 3.14159 + (1 - F) * NdotL 
	//		* pixelData.myMetalnessAlbedo.xyz )) * lightColor * attenuation;


	//	float lambert = dot(lightDir, pixelData.myNormal);

	//	
	//	float4 finalColor = SpotLights[0].Color;
	//	finalColor.w = 1.f;
	//	finalColor = saturate(lambert * finalColor);
	//	finalColor *= intensity;
	//	finalColor *= pixelData.myAlbedo;
	//	
	//	finalColor *= float4(directSpec.xyz, 1.f);
	//	
	//	finalColor *= attenuation;

	//	//return saturate(finalColor);	
	//float4 color = finalColor;


	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	float2 projectTexCoord;
	float4 projectionColor;

	projectTexCoord.x = input.ViewPosition.x / input.ViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.ViewPosition.y / input.ViewPosition.w / 2.0f + 0.5f;

	// Determine if the projected coordinates are in the 0 to 1 range.  If it is then this pixel is inside the projected view port.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		//Sample the projection texture using the projected texture coordinates and then set the output pixel color to be the projected texture color.
		// Sample the color value from the projection texture using the sampler at the projected texture coordinate location.
		projectionColor = DiffuseTexture.Sample(linearSampling, projectTexCoord);
		if (projectionColor.w == 0.f)
		{
			discard;
		}
		// Set the output color of this pixel to the projection texture overriding the regular color value.
		color = projectionColor;
		
	}
	else
	{
		discard;
	}
	return saturate(color);

	//}
	//else
	//{
	//	return float4(0.f, 0.f, 0.f, 0.f);
	//}
}

BlendState NoBlend
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};


BlendState BlendCorrect
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


BlendState BlendAlpha
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}