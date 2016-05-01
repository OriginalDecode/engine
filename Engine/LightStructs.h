#pragma once
#include <Math/Vector/Vector.h>

#define SPOTLIGHT_SIZE 3
#define POINTLIGHT_SIZE 3
#define DIRECTIONAL_SIZE 1

enum class eLightAxis
{
	XAxis,
	YAxis,
	ZAxis,
	NONE,
};


struct SSpotlightData
{
	CU::Vector4f myLightColor;
	CU::Vector3f myLightPosition;
	float myRange;
	CU::Vector3f myDirection;
	float myAngle;
};

struct SPointlightData
{
	CU::Vector4f myLightColor;
	CU::Vector3f myLightPosition;
	float myRange;
};

struct SDirectionallightData
{
	CU::Vector4f myLightColor;
	CU::Vector4f myLightDirection;
};