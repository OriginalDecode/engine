#include "S_variables_pbl.fx"
#include "S_light_directional.fx"
#include "S_light_point.fx"
#include "S_light_spot.fx"

float4 g_vTessellationFactor = float4(5.f, 12.f, 7.f, 1.f);

struct HS_OUTPUT
{
    float4 Position : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float2 Tex : TEXCOORD2;
    float3 Tangent : TEXCOORD3;
    float3 BiNormal :TEXCOORD4;  
	float4x4 WaterWorld : TEXCOORD5;
	float2 TexNonAnimated : POSITION;
};

struct DS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD1;
    float2 Tex : TEXCOORD2;
    float3 Tangent : TEXCOORD3;
    float3 BiNormal :TEXCOORD4;
	float Offset : POSITION;
	float2 Depth: TEXCOORD5;
	float2 ScreenTex: TEXCOORD6;
	float4 WVPPosition : TEXCOORD7;
	float2 TexNonAnimated : TEXCOORD8;
};

////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};
ConstantOutputType ColorPatchConstantFunction(InputPatch<PS_INPUT_1UVSET_WATER, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
	
	 float4 vEdgeTessellationFactors = g_vTessellationFactor.xxxy;
	// Calculate edge scale factor from vertex scale factor: simply compute 
    // average tess factor between the two vertices making up an edge
    vEdgeTessellationFactors.x = 0.5 * ( inputPatch[1].fVertexDistanceFactor + inputPatch[2].fVertexDistanceFactor );
    vEdgeTessellationFactors.y = 0.5 * ( inputPatch[2].fVertexDistanceFactor + inputPatch[0].fVertexDistanceFactor );
    vEdgeTessellationFactors.z = 0.5 * ( inputPatch[0].fVertexDistanceFactor + inputPatch[1].fVertexDistanceFactor );
    vEdgeTessellationFactors.w = vEdgeTessellationFactors.x;

    // Multiply them by global tessellation factor
	
    vEdgeTessellationFactors *= g_vTessellationFactor.xxxy;
		
	output.edges[0] = vEdgeTessellationFactors.x;
    output.edges[1] = vEdgeTessellationFactors.y;
    output.edges[2] = vEdgeTessellationFactors.z;
    output.inside  = vEdgeTessellationFactors.w;
	
    return output;
}

PS_INPUT_1UVSET_WATER VS_1UVSET(VS_INPUT_1UVSET_INSTANCED input)
{
	Matrix locWorld;
	locWorld._11_12_13_14 = input.world0;
	locWorld._21_22_23_24 = input.world1;
	locWorld._31_32_33_34 = input.world2;
	locWorld._41_42_43_44 = input.world3;

	//float offset = -abs(cos((TotalTime) - ((-input.Position.x * 4.f) - (input.Position.z * 4.f)) * 5.f) * 0.5f);

	//input.Position.y += offset;

	PBLVertexShaderData vertexData = CalcVertexShaderData_Instanced(input.Position, input.Normal
	, input.Tex, input.BiNormal, input.Tangent, locWorld, input.scale);

	PS_INPUT_1UVSET_WATER output = (PS_INPUT_1UVSET_WATER)0;
	output.Position = vertexData.Position;
	output.Normal = vertexData.Normal;
	output.Tex = vertexData.Tex;
	output.BiNormal = vertexData.BiNormal;
	output.Tangent = vertexData.Tangent;
	output.WorldPosition = vertexData.WorldPosition;
	output.WaterWorld = locWorld;
	//float phase = TotalTime * 3.0f;
 	//float4 wpos = vertexData.Position;
	//float offset = (wpos.x * 2.f + (wpos.z * 0.8f)) * 0.5f;
    //wpos.y += sin(phase + offset) * 1.f;
   	//output.Position += wpos;

	//output.Offset = mul(wpos.y, locWorld);

	const float fMinDistance = 0.0f;
    const float fMaxDistance = 600.0f;

    // Calculate distance between vertex and camera, and a vertex distance factor issued from it

    float3 cameraPositionYeah = float3(-10, 0, 30);
    float fDistance = distance(vertexData.WorldPosition.xyz, cameraPositionYeah);

    output.fVertexDistanceFactor = 0.53 - clamp(((fDistance - fMinDistance) / (fMaxDistance - fMinDistance)), 
    	0.0, 1.0 - g_vTessellationFactor.z / g_vTessellationFactor.x);

    // output.fVertexDistanceFactor = saturate(((1.0f - (fDistance / 70) ) * 2)) + (fDistance / 200); //old tesselation
    output.fVertexDistanceFactor = saturate(((1.0f - (fDistance / 20) ) * 2)) + (20 / fDistance); //new tesselation

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Hull Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]
HS_OUTPUT HS(InputPatch<PS_INPUT_1UVSET_WATER, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUTPUT output = (HS_OUTPUT)0;

    // Set the position for this control point as the output position.
    output.Position = patch[pointId].WorldPosition;
    // Set the input color as the output color.
	output.Tex = patch[pointId].Tex;
	output.TexNonAnimated = output.Tex;
	output.TexNonAnimated.y += cos(TotalTime * 3) * 0.00025f;
	output.Tex.x += (TotalTime * 0.001f);
	output.Tex.y -= (TotalTime * 0.001f);
	output.Normal =  patch[pointId].Normal;
    output.BiNormal = patch[pointId].BiNormal;
	output.Tangent  = patch[pointId].Tangent;
	output.WaterWorld = patch[pointId].WaterWorld;


    return output;
}


float2		g_WaterBumpTexcoordScale={7,7};
float		g_WaterHeightBumpScale=1.0f;
float		g_HeightFieldSize = 512;
float2 g_WaterBumpTexcoordShift = {1.f, 1.f};
// constructing the displacement amount and normal for water surface geometry
float4 CombineWaterNormal(float3 world_position, float2 aUV)
{
	float4 water_normal=float4(0.0,4.0,0.0,0.0);
	float water_miplevel = 0;
	float distance_to_camera;
	float4 texvalue;
	float texcoord_scale=1.0;
	float height_disturbance_scale = 1.0f;
	float normal_disturbance_scale = 1.0f;
	float2 tc;
	float2 variance= {1.0,1.0};

	// calculating MIP level for water texture fetches
	distance_to_camera=length(cameraPosition - world_position);
	water_miplevel= 0;
	tc=(world_position.xz * g_WaterBumpTexcoordScale / g_HeightFieldSize);

	// fetching water heightmap
	for(float i=0;i<5;i++)
	{
		texvalue = EmissiveTexture.SampleLevel(linearSampling, aUV,water_miplevel).rbga;
		variance.x*=-1.0;
		water_normal.xz+=(2*texvalue.xz-float2(1.0,1.0))*normal_disturbance_scale;
		water_normal.w += (texvalue.w-0.5)*height_disturbance_scale;
		texcoord_scale*=1.4;
		height_disturbance_scale*=0.65;
		normal_disturbance_scale*=0.65;
	}
	water_normal.w*=g_WaterHeightBumpScale;
	return float4(normalize(water_normal.xyz),water_normal.w);
}


////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
DS_OUTPUT DS(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
    float4 vertexPosition;
    DS_OUTPUT output = (DS_OUTPUT)0;
	
    // Determine the position of the new vertex.
	output.Tex = uvwCoord.x * patch[0].Tex + uvwCoord.y * patch[1].Tex + uvwCoord.z * patch[2].Tex;
	output.TexNonAnimated = uvwCoord.x * patch[0].TexNonAnimated + uvwCoord.y * patch[1].TexNonAnimated + uvwCoord.z * patch[2].TexNonAnimated;			  
	
	float4 releafTex = EmissiveTexture.SampleLevel(linearSampling, output.Tex, 0);
	float4 normalMap = NormalTexture.SampleLevel(linearSampling, output.Tex, 0);
	
    vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
	
	output.Normal = uvwCoord.x * patch[0].Normal + uvwCoord.y * patch[1].Normal + uvwCoord.z * patch[2].Normal; 
	output.Normal = normalize(output.Normal);
    output.Tangent = uvwCoord.x * patch[0].Tangent + uvwCoord.y * patch[1].Tangent + uvwCoord.z * patch[2].Tangent; 
    output.BiNormal = uvwCoord.x * patch[0].BiNormal + uvwCoord.y * patch[1].BiNormal + uvwCoord.z * patch[2].BiNormal; 


     float4x4 worldWorld = uvwCoord.x * patch[0].WaterWorld + uvwCoord.y * patch[1].WaterWorld + uvwCoord.z * patch[2].WaterWorld; 
   //	vertexPosition.y += cos((TotalTime) - vertexPosition.x * 0.8f) * 2.3f;

	float scale = 1.2;
	//scale = cos((TotalTime) - ((-vertexPosition.x * 5.f))) * 1;
	float4 depthmap_scaler = NormalTexture.SampleLevel(linearSampling, output.Tex,0);
	float4 water_normal = CombineWaterNormal(vertexPosition.xyz, output.Tex);

	water_normal.xyz = lerp(float3(0,1,0), normalize(water_normal.xyz), 0.4 + 0.6 * 5);
	output.Normal = normalize(water_normal.xyz);
	
	vertexPosition.y += cos(TotalTime * 2 - (vertexPosition.x) - vertexPosition.z) * 0.2f;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    Matrix WorldView = mul(worldWorld, ViewProjection);
    output.Position = mul(vertexPosition, WorldView);
	output.WVPPosition = output.Position;
	//output.Position.xyz = vertexPosition.xyz;
	output.Offset = abs(cos(TotalTime * 2 - (vertexPosition.x) - vertexPosition.z) * 0.3f);
	
//soft particles part
	output.Depth = output.Position.zw;
	output.ScreenTex = output.Position.xy / output.Position.w;

    return output;
}




float4 PS_1UVSET_DEPTHONLY(DS_OUTPUT input) : SV_Target
{
	discard;
	return float4(0,0,0,0);
}


float Contrast(float Input, float ContrastPower)
{
     bool IsAboveHalf = Input > 0.5 ;
     float ToRaise = saturate(2*(IsAboveHalf ? 1-Input : Input));
     float Output = 0.5*pow(ToRaise, ContrastPower); 
     Output = IsAboveHalf ? 1-Output : Output;
     return Output;
}


static const float g_fFadeDistance = 0.25;
GBuffer PS_1UVSET(DS_OUTPUT input) : SV_Target
{
	GBuffer buffer = CalculateGBuffer(input.Position, input.Tex, input.Normal, input.BiNormal, input.Tangent);
	buffer.Albedo.xyzw = Color.xyzw;
	//if(input.Offset < 0.5f)
	{
		buffer.Albedo.y += abs(input.Offset) * 0.2;

		buffer.Normal.w = abs(input.Offset) * 0.86; //metalness

		buffer.Depth.y = abs(input.Offset) * 0.01; //roughness
	}


//soft particles part


	// Const values
	float SoftParticleScale = 0.2;	
	float SoftParticleContrast = 1.0 ;
	float intensity = 7.0;
	float zEpsilon = 0.0f;
	
	float FAR_CLIP = 1000.0f;
	float NEAR_CLIP = 0.1f;
	
	float ZFARMULTZNEAR = NEAR_CLIP * FAR_CLIP;
	float ZFARMINUSZNEAR = FAR_CLIP - NEAR_CLIP ;
	                      
	float zBuf = DepthTexture.Load( int4(input.Position.x, input.Position.y,0,0)).r;
	float z = ZFARMULTZNEAR / ( FAR_CLIP - zBuf * ZFARMINUSZNEAR);
    float zdiff = (z - input.WVPPosition.z);
    float c = Contrast(zdiff * SoftParticleScale, SoftParticleContrast);
    if( c * zdiff <= zEpsilon )
    {
        discard;
    }
	
    //buffer.Albedo.xyz = saturate((1.0f - (c * (intensity)))) + buffer.Albedo.xyz;

	float4 foamTexture = EmissiveTexture.Sample(linearSampling, input.TexNonAnimated * 500) * saturate((1.0f - (c * (intensity))));

	buffer.Albedo.xyz += foamTexture;
	buffer.Emissive.xyz = abs(Color.xyz * 0.75f);
	return buffer;
}


technique11 Render
{
/*
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
	}
	*/
}

technique11 Render_1UVSET_INSTANCED
{
	pass P0
	{
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}

technique11 Render_1UVSET_INSTANCED_DEPTHONLY
{
	pass P0
	{
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetVertexShader(CompileShader(vs_5_0, VS_1UVSET()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_1UVSET_DEPTHONLY()));
		SetBlendState(BlendDefault, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}