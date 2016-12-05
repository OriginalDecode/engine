#include "stdafx.h"
#include "RenderCommand.h"
#include <DataStructures/Hashmap/Hash.h>
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
	, m_Orientation(rotationMatrix)
	, myCommandType(eCommandBuffer::eSpotlight)
{
}

RenderCommand::RenderCommand(const eType& aType, const CU::Vector3f& aPosition)
	: myType(aType)
	, myPosition(aPosition)
	, myCommandType(eCommandBuffer::e3D)
{
}

RenderCommand::RenderCommand(const eType& type, const std::string& sprite_key, const CU::Vector2f& position)
	: myType(type)
	, myPosition(position)
	, m_KeyOrText(sprite_key)
	, myCommandType(eCommandBuffer::e2D)
{
}

RenderCommand::RenderCommand(const eType& type, IShaderResourceView* shader_resource, const CU::Vector2f& position)
	: myType(type)
	, myPosition(position)
	, m_ShaderResource(shader_resource)
	, myCommandType(eCommandBuffer::e2D)
{
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

RenderCommand::RenderCommand(const eType& aType, const std::string& modelKey, const CU::Matrix44f& orientation)
	: myType(aType)
	, m_KeyOrText(modelKey)
	, m_Orientation(orientation)
	, myCommandType(eCommandBuffer::e3D)
{
}

RenderCommand::RenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& position)
	: myType(aType)
	, m_KeyOrText(modelKey)
	, myCommandType(eCommandBuffer::e3D)
{
	m_HashKey = Hash(m_KeyOrText.c_str());
	m_Orientation.SetPosition(position);
}

