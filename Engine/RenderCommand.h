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
	MODEL_NO_DEFERRED,
	LINE_Z_DISABLE,
	LINE_Z_ENABLE,
	SKYSPHERE,
	POINTLIGHT,
	SPOTLIGHT,
	PARTICLE,
	TERRAIN,
	WIREFRAME,
};

class Model;

struct RenderCommand
{
	RenderCommand() = default;
	RenderCommand(const eType& aType);

	RenderCommand(const eType& type, const std::string& sprite_key, const CU::Vector2f& position);
	RenderCommand(const eType& type, const std::string& text, const CU::Vector4f& color, const CU::Vector2f& position);
	RenderCommand(const eType& type, IShaderResourceView* shader_resource, const CU::Vector2f& position);

	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, float intensity, float range, bool shadow_casting);
	RenderCommand(const eType& aType, const CU::Vector3f& position, const CU::Vector3f& color, float angle
		, float range, const CU::Vector3f& direction, const CU::Matrix44f& rotationMatrix, int light_id, bool shadow_casting);


	//Should all of this data be copied instead of const ref since the memory is going to get lost the next frame anyway?
	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Matrix44f& orientation, const CU::Vector4f& scale = CU::Vector4f(1,1,1,1), bool wireframe = false);
	RenderCommand(const eType& aType, const std::string& modelKey, const CU::Vector3f& position);

	RenderCommand(const eType& aType, const CU::Vector3f& aPosition);
	RenderCommand(const eType& aType, const SLinePoint& aFirstPoint, const SLinePoint& aSecondPoint);

	IShaderResourceView* m_ShaderResource = nullptr;

	std::string m_KeyOrText;
	u64 m_HashKey;

	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	CU::Vector3f myDirection;
	CU::Vector4f m_Scale;

	CU::Matrix44f m_Orientation;

	union
	{
		float myAngle;
		float myIntensity;
	};

	float myRange;

	bool m_Wireframe = false;
	bool m_ShadowCasting = false;

	SLinePoint firstPoint;
	SLinePoint secondPoint;

	eCommandBuffer myCommandType;
	eType myType;
	int m_LightID;
};