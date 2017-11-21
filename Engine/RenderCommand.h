#pragma once
#include <Engine/engine_shared.h>
#include <string>
#include <CommonLib/Math/Matrix/Matrix.h>
#include <Engine/Line3D.h>
#include <Engine/Texture.h>
struct ID3D11ShaderResourceView;

struct RenderCommand
{
	enum eCommandType
	{
		NONE = -1,
		SHADOW,
		MODEL,
		SPOTLIGHT,
		POINTLIGHT,
		SPRITE,
		TEXT,
		LINE,
		PARTICLE,
		NOF_TYPES
	};


	RenderCommand(eCommandType command_type)
		: m_CommandType(command_type)
	{
	}

	eCommandType m_CommandType = NONE;
};

struct ModelCommand : public RenderCommand
{
	ModelCommand(u64 key, const CU::Matrix44f& orientation, bool wireframe)
		: RenderCommand(eCommandType::MODEL)
		, m_Orientation(orientation)
		, m_Wireframe(wireframe)
		, m_Key(key)
	{
	}

	ModelCommand(u64 model_key, u64 material_key, const CU::Matrix44f& orientation, bool wireframe)
		: RenderCommand(eCommandType::MODEL)
		, m_Key(model_key)
		, m_MaterialKey(material_key)
		, m_Orientation(orientation)
		, m_Wireframe(wireframe)
	{
	}

#ifdef _DEBUG
	ModelCommand(u64 model_key, u64 material_key, const CU::Matrix44f& orientation, bool wireframe, u32 entity_id)
		: RenderCommand(eCommandType::MODEL)
		, m_Key(model_key)
		, m_MaterialKey(material_key)
		, m_Orientation(orientation)
		, m_Wireframe(wireframe)
		, m_EntityID(entity_id)
	{
	}
#endif



	ModelCommand(u64 key, const CU::Vector3f& position, bool wireframe)
		: RenderCommand(eCommandType::MODEL)
		, m_Wireframe(wireframe)
		, m_Key(key)
	{
		m_Orientation.SetPosition(position);
	}

	ModelCommand(u64 key, const CU::Vector3f& position, bool wireframe, float roughness, float metalness) 
		: RenderCommand(eCommandType::MODEL)
		, m_Wireframe(wireframe)
		, m_Key(key)
		, m_Roughness(roughness)
		, m_Metalness(metalness)
	{
		m_Orientation.SetPosition(position);
	}

	ModelCommand(u64 key, u64 material_key, const CU::Vector3f& position, bool wireframe, float roughness, float metalness)
		: RenderCommand(eCommandType::MODEL)
		, m_Wireframe(wireframe)
		, m_Key(key)
		, m_MaterialKey(material_key)
		, m_Roughness(roughness)
		, m_Metalness(metalness)
	{
		m_Orientation.SetPosition(position);
	}

	ModelCommand(u64 key, u64 material_key, const CU::Vector3f& position, bool wireframe)
		: RenderCommand(eCommandType::MODEL)
		, m_Wireframe(wireframe)
		, m_Key(key)
		, m_MaterialKey(material_key)
	{
		m_Orientation.SetPosition(position);
	}

	u64 m_Key = 0;
	u64 m_MaterialKey = 0;
	CU::Matrix44f m_Orientation;
	bool m_Wireframe = false;
	float m_Roughness = 0.f;
	float m_Metalness = 0.f;
#ifdef _DEBUG
	u32 m_EntityID = 0;
#endif

};

struct ModelCommandNonDeferred : public ModelCommand
{
	ModelCommandNonDeferred(const u64& key, const CU::Matrix44f& orientation, bool wireframe)
		: ModelCommand(key, orientation, wireframe)
	{
	}
};

struct ShadowCommand : public RenderCommand
{
	ShadowCommand(u64 key, const CU::Matrix44f& orientation)
		: RenderCommand(eCommandType::SHADOW)
		, m_Orientation(orientation)
		, m_Key(key)
	{
	}

	u64 m_Key;
	CU::Matrix44f m_Orientation;
};

struct SpotlightCommand : public RenderCommand
{
	SpotlightCommand(s32 light_id, float angle, float range, float intensity, const CU::Vector4f& color, const CU::Matrix44f& orientation, bool shadow_casting)
		: RenderCommand(eCommandType::SPOTLIGHT)
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

struct PointlightCommand : public RenderCommand
{
	PointlightCommand(s32 light_id, float range, float intensity, const CU::Vector4f& color, const CU::Matrix44f& orientation)
		: RenderCommand(eCommandType::POINTLIGHT)
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

struct SpriteCommand : public RenderCommand
{
	SpriteCommand(u64& key, const CU::Vector2f& position)
		: RenderCommand(eCommandType::SPRITE)
		, m_Key ( key )
		, m_Position ( position ) 
	{
	}

	u64 m_Key;
	CU::Vector2f m_Position;
};


struct TextCommand : public RenderCommand
{
	TextCommand(const CU::Vector2f& position, std::string text)
		: RenderCommand(eCommandType::TEXT)
		, m_Position(position)
	{
		strcpy_s(m_TextBuffer, text.c_str());
	}

	char m_TextBuffer[255] = { '\0' };
	CU::Vector2f m_Position;
};
#define TextCommandA(position, ...) TextCommand(position, cl::HandleVAArgs(__VA_ARGS__))


struct LineCommand : public RenderCommand
{
	LineCommand(const LinePoint& first, const LinePoint& second, bool z_enabled)
		: RenderCommand(eCommandType::LINE)
		, m_Points{ first, second }
		, m_ZEnabled(z_enabled)
	{
	}

	LineCommand(const Line& line, bool z_enabled)
		: RenderCommand(eCommandType::LINE)
		, m_Points{ line.m_Points[0], line.m_Points[1] }
		, m_ZEnabled(z_enabled)
	{
	}


	bool m_ZEnabled = false;
	LinePoint m_Points[2];
};

struct ParticleCommand : public RenderCommand
{
	ParticleCommand(const CU::Vector3f& position)
		: RenderCommand(eCommandType::PARTICLE)
		, m_Position(position)
	{
	}

	CU::Vector3f m_Position;

};

