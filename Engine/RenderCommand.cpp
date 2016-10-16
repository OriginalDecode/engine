#include "stdafx.h"
#include "RenderCommand.h"

RenderCommand::RenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType)
	: myType(aType)
	, myTextToPrint(aString)
	, myPosition(aPosition)
	, myCommandType(eCommandBuffer::e2D)
{
}

RenderCommand::RenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& aPosition)
	: myType(aType)
	, myModelKey(modelKey)
	, myPosition(aPosition)
	, myCommandType(eCommandBuffer::e3D)
{
}

RenderCommand::RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range)
	: myType(aType)
	, myPosition(position)
	, myColor(color)
	, myIntensity(intensity)
	, myRange(range)
	, myCommandType(eCommandBuffer::ePointlight)
{
}

RenderCommand::RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& angle, const float& range, const CU::Vector3f& direction, const CU::Matrix44f& rotationMatrix)
	: myType(aType)
	, myPosition(position)
	, myColor(color)
	, myAngle(angle)
	, myRange(range)
	, myDirection(direction)
	, myRotationMatrix(rotationMatrix)
	, myCommandType(eCommandBuffer::eSpotlight)
{
}


RenderCommand::RenderCommand(const eType& aType, Snowblind::CEmitterInstance* anInstance)
	: myType(aType)
	, myEmitterInstance(anInstance)
	, myCommandType(eCommandBuffer::eParticle)
{
}


RenderCommand::RenderCommand(const eType& aType, const CU::Vector3f& aPosition)
	: myType(aType)
	, myPosition(aPosition)
	, myCommandType(eCommandBuffer::e3D)
{
}

RenderCommand::RenderCommand(const eType& aType, const std::string& aSpriteName, const CU::Vector2f& aPosition)
	: myType(aType)
	, myPosition(aPosition)
	, myCommandType(eCommandBuffer::e2D)
{
	aSpriteName;
}

RenderCommand::RenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint)
	: myType(aType)
	, firstPoint(aFirstPoint)
	, secondPoint(aSecondPoint)
	, myCommandType(eCommandBuffer::eLine)
{
}

RenderCommand::RenderCommand(const eType& aType)
	: myType(aType)
	, myCommandType(eCommandBuffer::e3D)
{
}

RenderCommand::RenderCommand()
{
}

