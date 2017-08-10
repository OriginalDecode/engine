cbuffer Matrices : register(b0) 
{
	row_major float4x4 ViewProjection;
    float4 Direction;
};

struct gsInput
{
	float4 pos : SV_POSITION;
	float2 range : RANGE;
	float2 dummy : DUMMY;
};

struct gsOut
{
    float4 m_Position : SV_POSITION;
	float4 uv : TEXCOORD;
    float m_InterpolDist : DISTANCE;
	float range : RANGE;
	float4 normal : NORMAL;
};


[maxvertexcount(12)]
void main(point gsInput anInput[1], inout TriangleStream<gsOut> triStream) 
{	
	gsInput input = anInput[0];
	float spotlength = 12;
	float halfwidth = 5; //attenuation?
	
	float3 spot_normal = Direction * spotlength;
	float3 right = normalize(cross(spot_normal, float3(0,1,0))) * halfwidth;
	float3 down = normalize(cross(spot_normal, right)) * halfwidth;

    gsOut output = (gsOut)0;
//0
    output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
	output.normal = float4(spot_normal,1);
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 1.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz - down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz + down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);
	
	triStream.RestartStrip();


 	output.m_Position = float4(input.pos.xyz + down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 1.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz + down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);


	triStream.RestartStrip();


 	output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz - down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz + down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz + down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);





/*	output.m_Position = float4(input.pos.xyz + down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz - down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);
	
	output.m_Position = float4(input.pos.xyz + down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);*/
}