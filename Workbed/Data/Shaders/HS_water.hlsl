struct VS_OUTPUT
{
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;
};

struct HS_OUTPUT
{
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;	
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

ConstantOutputType ColorPatchConstantFunction(InputPatch<VS_OUTPUT, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
    int  tessellationAmount = 2; 
    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;
	output.edges[3] = tessellationAmount;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside[0] = tessellationAmount;
	output.inside[1] = tessellationAmount;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ColorPatchConstantFunction")]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HS_OUTPUT output = (HS_OUTPUT)0;
	output.pos = patch[pointId].pos;
	output.normal = patch[pointId].normal;
	output.uv = patch[pointId].uv;
	output.binorm = patch[pointId].binorm;
	output.tang = patch[pointId].tang;
	output.worldpos = patch[pointId].worldpos;

	return output;
}
