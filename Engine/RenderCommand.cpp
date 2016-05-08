#include "stdafx.h"
#include "RenderCommand.h"



SRenderCommand::SRenderCommand()
{
}

SRenderCommand::SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType)
	: myTextToPrint(aString)
	, myPosition(aPosition)
	, myType(aType)
	, myCommandType(eCommandType::e2D)
{
}

SRenderCommand::SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType)
	: myInstance(anInstance)
	, myPosition(aPosition)
	, myType(aType)
	, myCommandType(eCommandType::e3D)
{
}

SRenderCommand::SRenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range)
	: myType(aType)
	, myPosition(position)
	, myColor(color)
	, myIntensity(intensity)
	, myRange(range)
	, myCommandType(eCommandType::LIGHT)
{
}
