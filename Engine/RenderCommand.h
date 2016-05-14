#pragma once
#include <string>
#include <Math/Vector/Vector.h>

namespace Snowblind
{
	class CInstance;
	class CPointLight;
	class CEmitterInstance;
}

enum class eCommandType
{
	e2D,
	e3D,
	LIGHT,
	PARTICLE,
};


struct SRenderCommand
{
	enum class eType
	{
		TEXT,
		MODEL,
		POINTLIGHT,
		PARTICLE,
	};
	SRenderCommand();
	SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType = eType::TEXT);
	SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType);
	SRenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range );
	SRenderCommand(Snowblind::CEmitterInstance* anInstance);
	Snowblind::CInstance* myInstance;
	Snowblind::CEmitterInstance* myEmitterInstance;
	//Snowblind::CPointLight* myPointLight;
	std::string myTextToPrint;
	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	eCommandType myCommandType;

	float myRange;
	float myIntensity;

	eType myType;

};

