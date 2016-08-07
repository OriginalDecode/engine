#pragma once
#include <string>
#include <Math/Vector/Vector.h>
#include "Line3D.h"
namespace Snowblind
{
	class CInstance;
	class CPointLight;
	class CEmitterInstance;
	class CSkySphere;
}

enum class eCommandType
{
	e2D,
	e3D,
	LIGHT,
	PARTICLE,
	LINE,
};

enum class eDeferredFlag
{
	IS_DEFERRED,
	NOT_DEFERRED,
};


struct SRenderCommand
{
	enum class eType
	{
		TEXT,
		SPRITE,
		MODEL,
		LINE,
		SKYSPHERE,
		POINTLIGHT,
		PARTICLE,
		TERRAIN,
	};

	SRenderCommand();
	SRenderCommand(const eType& aType);
	SRenderCommand(const eType& aType, const std::string& aSpriteName, const CU::Vector2f& aPosition);
	SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType = eType::TEXT);
	SRenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range );
	SRenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& aPosition);
	SRenderCommand(const eType& aType, Snowblind::CEmitterInstance* anInstance);
	SRenderCommand(const eType& aType,  const CU::Vector3f& aPosition);
	SRenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint);

	Snowblind::CEmitterInstance* myEmitterInstance;

	std::string myTextToPrint;
	std::string myModelKey;
	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	eCommandType myCommandType;
	float myRange;
	float myIntensity;
	
	SLinePoint firstPoint;
	SLinePoint secondPoint;

	eType myType;
};

