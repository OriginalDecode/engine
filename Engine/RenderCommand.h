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
	LINE,
	SKYSPHERE,
	POINTLIGHT,
	SPOTLIGHT,
	PARTICLE,
	TERRAIN,
};

struct SRenderCommand
{
	SRenderCommand();
	SRenderCommand(const eType& aType);
	SRenderCommand(const eType& aType, const std::string& aSpriteName, const CU::Vector2f& aPosition);
	SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType = eType::TEXT);

	SRenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range);
	SRenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& angle, const float& range, const CU::Vector3f& direction, const CU::Matrix44f& rotationMatrix);

	SRenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& aPosition);
	SRenderCommand(const eType& aType, Snowblind::CEmitterInstance* anInstance);
	SRenderCommand(const eType& aType, const CU::Vector3f& aPosition);
	SRenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint);

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

