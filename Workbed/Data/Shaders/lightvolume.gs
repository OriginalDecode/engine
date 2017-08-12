cbuffer Matrices : register(b0) 
{
	row_major float4x4 ViewProjection;
};

struct gsInput
{
	float4 pos : SV_POSITION;
	float range : RANGE;
	float angle : ANGLE;
	float cosAngle : ANGLE;
	float3 direction : DIRECTION;
};

struct gsOut
{
    float4 m_Position : SV_POSITION;
    float m_InterpolDist : DISTANCE;
	float range : RANGE;
	float cosAngle : ANGLE;
	float4 normal : NORMAL;
	float4 direction : DIRECTION;
};


float CalculateAttenuation(float someDistance)
{
	float attenuation = 1.f / (1.f + 0.1f * someDistance + 0.01f * someDistance * someDistance);
	return attenuation;
};

float CalculateFalloff(float someDistance, float someRange)
{
	float fallOff = 0.9f - (someDistance / (someRange + 0.00001f));
	return fallOff;
};

float CalculateTotalAttenuation(float someDistance, float someRange)
{
	float totalAttenuation = CalculateAttenuation(someDistance) * CalculateFalloff(someDistance, someRange);
	return totalAttenuation;
};

[maxvertexcount(8)]
void spotlight(point gsInput anInput[1], inout TriangleStream<gsOut> triStream) 
{	
	gsInput input = anInput[0];

	float ln = length( input.pos - (input.pos + (float4(input.direction,1) * input.range)) );

	
	float spotlength = input.range;
	float halfwidth = 5;//CalculateTotalAttenuation(ln, input.range);
	
	float3 spot_normal = input.direction * spotlength;
	float3 right = normalize(cross(spot_normal, float3(0,1,0))) * halfwidth;
	float3 down = normalize(cross(spot_normal, right)) * halfwidth;

    gsOut output = (gsOut)0;

	output.range = input.range;
	output.cosAngle = 0.9f;//input.cosAngle;
	output.direction = float4(input.direction,1);


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
	


 	output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

 	output.m_Position = float4(input.pos.xyz + down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 1.f;
    triStream.Append(output);

	output.m_Position = float4(input.pos.xyz + down + right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;
    triStream.Append(output);


	triStream.RestartStrip();
}