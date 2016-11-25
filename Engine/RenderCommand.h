#pragma once
#include <string>
#include <Math/Vector/Vector.h>
#include "Line3D.h"
#include "RenderCommand_Shared.h"
namespace Snowblind
{
	class CInstance;
	class CPointLight;
	class CEmitterInstance;
	class CSkySphere;
}

enum class eDeferredFlag
{
	IS_DEFERRED,
	NOT_DEFERRED,
};


enum class eType
{
	TEXT,
	SPRITE,
	MODEL,
	LINE_Z_DISABLE,
	LINE_Z_ENABLE,
	SKYSPHERE,
	POINTLIGHT,
	SPOTLIGHT,
	PARTICLE,
	TERRAIN,
};

struct RenderCommand
{
	RenderCommand() = default;
	RenderCommand(const eType& aType);
	RenderCommand(const eType& aType, const std::string& aSpriteName, const CU::Vector2f& aPosition);
	RenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType = eType::TEXT);

	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range);
	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& angle, const float& range, const CU::Vector3f& direction, const CU::Matrix44f& rotationMatrix);

	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& aPosition);
	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Matrix44f& orientation);

	RenderCommand(const eType& aType, Snowblind::CEmitterInstance* anInstance);
	RenderCommand(const eType& aType, const CU::Vector3f& aPosition);
	RenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint);

	Snowblind::CEmitterInstance* myEmitterInstance;

	std::string myTextToPrint;
	std::string myModelKey;
	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	CU::Vector3f myDirection;
	CU::Matrix44f myRotationMatrix;

	union
	{
		float myAngle;
		float myIntensity;
	};

	float myRange;

	SLinePoint firstPoint;
	SLinePoint secondPoint;

	eCommandBuffer myCommandType;
	eType myType;
};

