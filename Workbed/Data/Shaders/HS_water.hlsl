cbuffer Matrices : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
};
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

struct VS_OUTPUT
{
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float fVertexDistanceFactor : TEXCOORD1;
};
float2		g_WaterBumpTexcoordScale={7,7};
float		g_WaterHeightBumpScale=1.0f;
float		g_HeightFieldSize = 512;
float2 g_WaterBumpTexcoordShift = {1.f, 1.f};
// constructing the displacement amount and normal for water surface geometry
struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};
ConstantOutputType ColorPatchConstantFunction(InputPatch<VS_OUTPUT, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;

	float4 vEdgeTessellationFactors = g_vTessellationFactor.xxxy;
	// Calculate edge scale factor from vertex scale factor: simply compute 
	// average tess factor between the two vertices making up an edge
	vEdgeTessellationFactors.x = 0.5 * (inputPatch[1].fVertexDistanceFactor + inputPatch[2].fVertexDistanceFactor);
	vEdgeTessellationFactors.y = 0.5 * (inputPatch[2].fVertexDistanceFactor + inputPatch[0].fVertexDistanceFactor);
	vEdgeTessellationFactors.z = 0.5 * (inputPatch[0].fVertexDistanceFactor + inputPatch[1].fVertexDistanceFactor);
	vEdgeTessellationFactors.w = vEdgeTessellationFactors.x;

	// Multiply them by global tessellation factor

	vEdgeTessellationFactors *= g_vTessellationFactor.xxxy;

	output.edges[0] = vEdgeTessellationFactors.x;
	output.edges[1] = vEdgeTessellationFactors.y;
	output.edges[2] = vEdgeTessellationFactors.z;
	output.inside = vEdgeTessellationFactors.w;

	return output;
}


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUTPUT output = (HS_OUTPUT)0;

    // Set the position for this control point as the output position.
    output.Position = patch[pointId].worldpos;
    // Set the input color as the output color.
	output.Tex = patch[pointId].uv;
	float TotalTime = 1.f;
	output.TexNonAnimated = output.Tex;
	output.TexNonAnimated.y += cos(TotalTime * 3) * 0.00025f;
	output.Tex.x += (TotalTime * 0.001f);
	output.Tex.y -= (TotalTime * 0.001f);
	output.Normal =  patch[pointId].normal;
    output.BiNormal = patch[pointId].binorm;
	output.Tangent  = patch[pointId].tang;
	//output.WaterWorld = patch[pointId].WaterWorld;

    return output;
}
