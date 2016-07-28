//---------------------------------
//	Deferred Ambient Pixel Shaders
//---------------------------------
cbuffer CameraPosition : register(b0)
{
	float4 camPosition;
	row_major float4x4 InvertedProjection;
	row_major float4x4 View;
};
//---------------------------------
//	Samplers & Textures
//---------------------------------

SamplerState point_Clamp 	: register ( s0 );
Texture2D AlbedoTexture  	: register ( t0 );
Texture2D NormalTexture  	: register ( t1 );
Texture2D DepthTexture	 	: register ( t2 );
TextureCube CubeMap		 	: register ( t3 );

//---------------------------------
//	Deferred Ambient Pixel Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION0;
	float2 uv	: TEXCOORD;
};

struct PixelData
{
	float4 albedo;
	float4 normal;
	float4 depth;
	float4 metalness;
	float4 roughness;
	float4 AOTexture;
	
	float4 emissive;
	
};


//---------------------------------
//	Deferred Ambient Pixel Shader
//---------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 albedo = AlbedoTexture.Sample(point_Clamp, input.uv);	
	float4 normal = NormalTexture.Sample(point_Clamp, input.uv);
	float4 depth = DepthTexture.Sample(point_Clamp, input.uv);
	normal.xyz *= 2.0f;
	normal.xyz -= 1.f;
	float4 cubemap = CubeMap.SampleLevel(point_Clamp, normal, 7); //Use in the future.
	
	
	float metalness = normal.w;
	float roughness = depth.y;
	
	float x = input.uv.x * 2.f - 1.f;
	float y = (1.f - input.uv.y) * 2.f - 1.f;
	float z = depth; 	
	
	float4 worldPosition = float4(x, y, z, 1.f);
	worldPosition = mul (worldPosition, InvertedProjection);
	worldPosition = worldPosition / worldPosition.w;
	worldPosition = mul(worldPosition, View);	
	
	float3 viewPos = camPosition.xyz;
	//float3 toEye = normalize(viewPos- /* World Pos Math */)
	

	float4 col = saturate(albedo * cubemap);
	//col.rgb = pow (col.rgb, 1 / 2.2);
	
	return col;
	

};

























