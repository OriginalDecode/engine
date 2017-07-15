//---------------------------------
//---------------------------------

SamplerState sampler0 : register ( s0 );
Texture2D DepthTexture : register ( t7 );
//---------------------------------
//---------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float2 alpha : ALPHA;
	float2 uv : TEXCOORD;
	float2 depth : DEPTH;
	float4 eyePos : POSITION0;
	float4x4 projection : MATRIX0;
	
};
cbuffer Matrices : register(b0)
{
	row_major float4x4 Projection;
};
//---------------------------------
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	float r = 8;
	float3 normal = float3(input.uv.xy  * 2 - 1, 1);


	float r2 = dot(normal.xy, normal.xy);
	if(r2 > 1.0)
		discard;

	normal.z = -sqrt(1.0 - r2);	

	float4 pixel = float4(input.eyePos.xyz + normal * r, 1.0);
	float4 clip = mul(pixel, Projection);
	float depth = clip.z / clip.w;
	
	float far = 0.999f;
	float near = 0.01f;


	//depth = (((far - near) / 2.f) * depth) + ((far + near) / 2);

	float3 light_dir = float3(0,1,0);
	float ndotl = dot(light_dir, normal);
	float4 output = float4(ndotl, ndotl, ndotl,ndotl) * float4(1,0,1,1);
	return float4(depth,depth,depth,depth);
};
