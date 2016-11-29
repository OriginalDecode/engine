struct VS_OUTPUT
{
	float4 worldpos : SV_POSITION;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 output = input.worldpos.zzzz;
	return output;
}
