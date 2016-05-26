#pragma once
#include <Math/Vector/Vector.h>
struct SVertexTypePosCol
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector4<float> myColor;
};

struct SVertexTypePosUV
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector2<float> myUV;
};

struct SVertexTypePosColUv
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector4<float> myColor;
	CU::Math::Vector2<float> myUV;
};

struct SVertexTypePosColUv2
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector4<float> myColor;
	CU::Math::Vector2<float> myUV;
	CU::Math::Vector2<float> myUV2;
};


struct SVertexTypePosNormUV
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector3<float> myNormal;
	CU::Math::Vector2<float> myUV;
};
