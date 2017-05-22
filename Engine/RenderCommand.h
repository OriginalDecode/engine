#pragma once
#include <string>
#include <Math/Vector/Vector.h>
#include "Line3D.h"
#include "RenderCommand_Shared.h"
#include "LightStructs.h"

typedef struct ID3D11ShaderResourceView IShaderResourceView;

struct RenderCommand2
{
	enum eCommandType
	{
		NONE = -1,
		MODEL,
		SPOTLIGHT,
		POINTLIGHT,
		NOF_TYPES
	};
	
	RenderCommand2(eCommandType command_type)
		: m_CommandType(command_type)
	{
	}

	eCommandType m_CommandType = NONE;
};

struct ModelCommand : public RenderCommand2
{
	ModelCommand(const std::string& key, const CU::Matrix44f& orientation, bool wireframe)
		: RenderCommand2(eCommandType::MODEL)
		, m_Orientation(orientation)
		, m_Wireframe(wireframe)
	{
		strcpy_s(m_Key, key.c_str());
	}
	
	char m_Key[128] = { '\0' };
	CU::Matrix44f m_Orientation;
	bool m_Wireframe = false;

};

struct SpotlightCommand : public RenderCommand2
{
	SpotlightCommand(s32 light_id, float angle, float range, float intensity, const CU::Vector4f& color, const CU::Matrix44f& orientation, bool shadow_casting)
		: RenderCommand2(eCommandType::SPOTLIGHT)
		, m_LightID(light_id)
		, m_Angle(angle)
		, m_Range(range)
		, m_Intensity(intensity)
		, m_Color(color)
		, m_Orientation(orientation)
		, m_ShadowCasting(shadow_casting)
	{
	}

	float m_Angle = 0.f;
	float m_Range = 0.f;
	float m_Intensity = 0.f;
	CU::Vector4f m_Color;
	CU::Matrix44f m_Orientation;
	s32 m_LightID = 0;
	bool m_ShadowCasting = false;
};

struct PointlightCommand : public RenderCommand2
{
	PointlightCommand(s32 light_id, float range, float intensity, const CU::Vector4f& color, const CU::Matrix44f& orientation)
		: RenderCommand2(eCommandType::POINTLIGHT)
		, m_LightID(light_id)
		, m_Range(range)
		, m_Intensity(intensity)
		, m_Color(color)
		, m_Orientation(orientation)
	{
	}

	float m_Range = 0.f;
	float m_Intensity = 0.f;
	CU::Vector4f m_Color;
	CU::Matrix44f m_Orientation;
	s32 m_LightID = 0;
};





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