#pragma once
#include <string>
#include <Math/Vector/Vector.h>
#include "Line3D.h"
#include "RenderCommand_Shared.h"

typedef struct ID3D11ShaderResourceView IShaderResourceView;

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

	RenderCommand(const eType& type, const std::string& sprite_key, const CU::Vector2f& position);
	RenderCommand(const eType& type, IShaderResourceView* shader_resource, const CU::Vector2f& position);

	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& intensity, const float& range);
	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, const float& angle, const float& range, const CU::Vector3f& direction, const CU::Matrix44f& rotationMatrix);

	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Matrix44f& orientation);
	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& position);


	RenderCommand(const eType& aType, const CU::Vector3f& aPosition);
	RenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint);

	IShaderResourceView* m_ShaderResource = nullptr;

	std::string m_KeyOrText;

	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	CU::Vector3f myDirection;

	CU::Matrix44f m_Orientation;

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