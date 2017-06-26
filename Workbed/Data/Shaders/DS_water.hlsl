cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
};

float4 g_vTessellationFactor = float4(5.f, 12.f, 7.f, 1.f);
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
struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

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
	distance_to_camera=length(float4(0,0,0,1) - world_position);
	water_miplevel= 0;
	tc=(world_position.xz * g_WaterBumpTexcoordScale / g_HeightFieldSize);

	// fetching water heightmap
	for(float i=0;i<5;i++)
	{
		texvalue = 1;// EmissiveTexture.SampleLevel(linearSampling, aUV, water_miplevel).rbga;
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


[domain("tri")]
DS_OUTPUT DS(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	float4 vertexPosition;
    DS_OUTPUT output = (DS_OUTPUT)0;
	
    // Determine the position of the new vertex.
	output.Tex = uvwCoord.x * patch[0].Tex + uvwCoord.y * patch[1].Tex + uvwCoord.z * patch[2].Tex;
	output.TexNonAnimated = uvwCoord.x * patch[0].TexNonAnimated + uvwCoord.y * patch[1].TexNonAnimated + uvwCoord.z * patch[2].TexNonAnimated;			  
	
	float4 releafTex = 1;// EmissiveTexture.SampleLevel(linearSampling, output.Tex, 0);
	float4 normalMap = float4(0, 1, 0, 1);// NormalTexture.SampleLevel(linearSampling, output.Tex, 0);
	
    vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
	
	output.Normal = uvwCoord.x * patch[0].Normal + uvwCoord.y * patch[1].Normal + uvwCoord.z * patch[2].Normal; 
	output.Normal = normalize(output.Normal);
    output.Tangent = uvwCoord.x * patch[0].Tangent + uvwCoord.y * patch[1].Tangent + uvwCoord.z * patch[2].Tangent; 
    output.BiNormal = uvwCoord.x * patch[0].BiNormal + uvwCoord.y * patch[1].BiNormal + uvwCoord.z * patch[2].BiNormal; 


    float4x4 worldWorld = uvwCoord.x * patch[0].WaterWorld + uvwCoord.y * patch[1].WaterWorld + uvwCoord.z * patch[2].WaterWorld; 
   	//vertexPosition.y += cos((TotalTime) - vertexPosition.x * 0.8f) * 2.3f;
	float TotalTime = 1.f;
	float scale = 1.2;
	//scale = cos((TotalTime) - ((-vertexPosition.x * 5.f))) * 1;
	float4 water_normal = CombineWaterNormal(vertexPosition.xyz, output.Tex);

	water_normal.xyz = lerp(float3(0,1,0), normalize(water_normal.xyz), 0.4 + 0.6 * 5);
	output.Normal = normalize(water_normal.xyz);
	
	vertexPosition.y += cos(TotalTime * 2 - (vertexPosition.x) - vertexPosition.z) * 0.2f;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    Matrix WorldView = mul(worldWorld, View);
	WorldView = mul(WorldView, Projection);
    output.Position = mul(vertexPosition, WorldView);
	output.WVPPosition = output.Position;
	//output.Position.xyz = vertexPosition.xyz;
	output.Offset = abs(cos(TotalTime * 2 - (vertexPosition.x) - vertexPosition.z) * 0.3f);
	
	//soft particles part
	output.Depth = output.Position.zw;
	output.ScreenTex = output.Position.xy / output.Position.w;

    return output;
}