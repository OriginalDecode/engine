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
};


[maxvertexcount(7)]
void main(point gsInput anInput[1], inout TriangleStream<gsOut> triStream) 
{	
	gsInput input = anInput[0];
	float spotlength = input.range.x;
	float halfwidth = 1; //attenuation?
	
	float3 spot_normal = Direction * spotlength;
	float3 right = normalize(cross(spot_normal, float3(0,1,0))) * halfwidth;
	float3 down = normalize(cross(spot_normal, right)) * halfwidth;

    gsOut output = (gsOut)0;

    output.m_Position = float4(input.pos.xyz - down - right + spot_normal, 1);
	output.m_Position = mul(output.m_Position, ViewProjection);
    output.m_InterpolDist = 0.f;

    triStream.Append(output);


	/*
	
	//0
	gl_Position = ViewProjMatrix * vec4(spot_pos - down - right + spot_normal, 1);
	interpolDist = 0.0f;
	EmitVertex();
	//1
	gl_Position = ViewProjMatrix * vec4(spot_pos, 1);
	interpolDist = 1.0f;
	EmitVertex();
	//2
	gl_Position = ViewProjMatrix * vec4(spot_pos - down + right + spot_normal, 1);
	interpolDist = 0.0f;
	EmitVertex();
	//3
	gl_Position = ViewProjMatrix * vec4(spot_pos + down + right + spot_normal, 1);
	EmitVertex();
	//4
	gl_Position = ViewProjMatrix * vec4(spot_pos - down - right + spot_normal, 1);
	EmitVertex();
	//5
	gl_Position = ViewProjMatrix * vec4(spot_pos + down - right + spot_normal, 1);
	EmitVertex();
	//6
	gl_Position = ViewProjMatrix * vec4(spot_pos, 1);
	interpolDist = 1.0f;
	EmitVertex();	
	//7
	gl_Position = ViewProjMatrix * vec4(spot_pos + down + right + spot_normal, 1);
	interpolDist = 0.0f;
	EmitVertex();
	
	EndPrimitive();*/
	
}