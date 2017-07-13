//---------------------------------
//	Render to Texture Vertex Shader
//---------------------------------
//---------------------------------
//	Render to Texture Vertex Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD;
};

//---------------------------------
//	Render to Texture Vertex Shader
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 depth = DepthTexture.Sample(sampler0, input.uv);


	half x = input.uv.x * 2.0h - 1.0h;
	half y = ( 1.0h - input.uv.y) * 2.0h - 1.0h;
	half z = depth.r;

	half3 uvToEye = half3(x, y, z);

	half3 ddx0 = DepthTexture.Sample(sampler0, input.uv + float2(1,0)) - uvToEye;
	half3 ddx1 = uvToEye -  DepthTexture.Sample(sampler0, input.uv + float2(0,1));
	if(abs(ddx0.z) > abs(ddx1.z))
		ddx0 = ddx1;

	half3 ddy0 = DepthTexture.Sample(sampler0, input.uv + float2(-1,0)) - uvToEye;
	half3 ddy1 = uvToEye -  DepthTexture.Sample(sampler0, input.uv + float2(0,-1));
	if(abs(ddy0.z) > abs(ddy1.z))
		ddy0 = ddy1;

	float3 n = normalize(cross(ddx0, ddy0));

	return float4(n, 1);
};