#pragma once
#include <Math/Vector/Vector.h>

#define SPOTLIGHT_SIZE 3
#define POINTLIGHT_SIZE 4
#define DIRECTIONAL_SIZE 1

enum class eLightAxis
{
	XAxis,
	YAxis,
	ZAxis,
	NONE,
};


struct SpotlightData
{
	CU::Matrix44f myOrientation;
	CU::Vector4f myLightColor;
	CU::Vector3f myLightPosition;
	float myRange;
	CU::Vector3f myDirection;
	float myAngle;
	float m_Intensity = 0.f;
};

struct SPointlightData
{
	CU::Vector4f myLightColor;
	CU::Vector4f myLightPosition;
	float myRange;
};

struct SDirectionallightData
{
	CU::Vector4f myLightColor;
	CU::Vector4f myLightDirection;
};