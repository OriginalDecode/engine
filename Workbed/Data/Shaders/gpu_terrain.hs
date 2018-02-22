struct VS_OUTPUT
{
	float4 pos : POSITION0;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binorm : BINORMAL;
	float3 tang : TANGENT;
	float4 worldpos : POSITION1;
	float4 camera_pos : POSITION2;
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
    float2 uv[4] : TEXCOORD;
    float inside[2] : SV_InsideTessFactor;
};


float CalcFactor(float3 p, float3 eye)
{
	const float minDist = 10;
	const float maxDist = 250;

	float d = distance(p, eye);
	float s = saturate( ( d - minDist) / (maxDist - minDist));
	return pow(2, (lerp(64, 2, s)));
};


ConstantOutputType ColorPatchConstantFunction(InputPatch<VS_OUTPUT, 4> patch, uint patchId : SV_PrimitiveID)
{    
	/* This will be rewritten to account the distance to camera too */

	float3 cam_pos = patch[0].camera_pos.xyz;
    ConstantOutputType output;

	float3 pos = float3(512,0,512);
	float test = CalcFactor(pos, cam_pos);

    const int outer_factor = test; //64 max 
    const int inner_factor = test; //64 max 
    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = outer_factor;
    output.edges[1] = outer_factor;
    output.edges[2] = outer_factor;
	output.edges[3] = outer_factor;

	output.uv[0] = outer_factor; 
	output.uv[1] = outer_factor; 
	output.uv[2] = outer_factor;
	output.uv[3] = outer_factor;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside[0] = inner_factor;
	output.inside[1] = inner_factor;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ColorPatchConstantFunction")]
HS_OUTPUT main(InputPatch<VS_OUTPUT, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
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
