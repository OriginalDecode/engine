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
	float4 center_radius : POSITION1;
	
};
cbuffer Matrices : register(b0)
{
	row_major float4x4 Projection;
};
//---------------------------------
//---------------------------------

float4 PS(VS_OUTPUT input) : SV_Target
{
	
	float far = 0.999f;
	float near = 0.01f;

	float dist = length(input.uv - float2(0.5,0.5));
	float alpha = saturate(sign(0.5 - dist));

	float3 _normal = float3(input.uv.xy  * 2 - 1, 1);
	float mag = dot(_normal.xy, _normal.xy);

	if(mag > 1.0)
		discard;


	float sphereDepth = cos(dist * 3.141559) * 1 * 1;
	float _depth = saturate(sphereDepth);
	_depth = (((far - near) / 2.f) * _depth) + ((far + near) / 2);
	return float4(_depth, _depth, _depth, alpha);



	float r = 0.5;
	float3 normal = float3(input.uv.xy  * 2 - 1, 1);
	float r2 = dot(normal.xy, normal.xy);
	
	float2 coord = ((input.uv.xy) * input.center_radius.xy )/ input.center_radius.z;
	float l = length(coord);

	if(l > 1.0)
		discard;

	float3 pos = float3(coord, sqrt(1.0 - (l * l)));
	return float4(pos.z, pos.z, pos.z, 1);


	normal.z = -sqrt(1.0 - r2);	

	float4 pixel = float4(input.eyePos.xyz + normal * r, 1.0);
	float4 clip = mul(pixel, Projection);
	float depth = clip.z / clip.w;



	//depth = (((far - near) / 2.f) * depth) + ((far + near) / 2);

	return float4(depth,depth,depth,depth);
};
