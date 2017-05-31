#pragma once
#include <string>
#include <CommonLib/Math/Matrix/Matrix.h>
#include <Engine/Line3D.h>
#include <Engine/ModelInstance.h>
struct ID3D11ShaderResourceView;

struct RenderCommand
{
	enum eCommandType
	{
		NONE = -1,
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
	ModelCommand(const ModelInstance& model_instance, const CU::Matrix44f& orientation, bool wireframe)
		: RenderCommand(eCommandType::MODEL)
		, m_Orientation(orientation)
		, m_Wireframe(wireframe)
		, m_ModelInstance(model_instance)
	{
	}
	
	const ModelInstance& m_ModelInstance;
	CU::Matrix44f m_Orientation;
	bool m_Wireframe = false;

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
	SpriteCommand(ID3D11ShaderResourceView* pResource, const CU::Vector2f& position)
		: RenderCommand(eCommandType::SPRITE)
		, m_Resource(pResource)
		, m_Position(position)
	{
	}

	SpriteCommand(const std::string& key, const CU::Vector2f& position)
		: RenderCommand(eCommandType::SPRITE)
		, m_Position(position)
	{
		strcpy_s(m_Key, key.c_str());
	}

	char m_Key[128] = { '\0' };
	ID3D11ShaderResourceView* m_Resource = nullptr;
	CU::Vector2f m_Position;
};

struct TextCommand : public RenderCommand
{
	TextCommand(const std::string& text, const CU::Vector2f& position, const CU::Vector4f& color)
		: RenderCommand(RenderCommand::TEXT)
		, m_Position(position)
		, m_Color(color)
	{
		strcpy_s(m_TextBuffer, text.c_str());
	}

	TextCommand(const std::string& text, const CU::Vector2f& position)
		: RenderCommand(RenderCommand::TEXT)
		, m_Position(position)
	{
		strcpy_s(m_TextBuffer, text.c_str());
		m_Color = { 1.f, 1.f, 1.f, 1.f };
	}

	char m_TextBuffer[255] = { '\0' };
	CU::Vector2f m_Position;
	CU::Vector4f m_Color;

};


struct LineCommand : public RenderCommand
{
	LineCommand(const SLinePoint& first, const SLinePoint& second, bool z_enabled)
		: RenderCommand(RenderCommand::LINE)
		, m_Points{ first, second }
		, m_ZEnabled(z_enabled)
	{
	}

	bool m_ZEnabled = false;
	SLinePoint m_Points[2];
};

struct ParticleCommand : public RenderCommand
{
	ParticleCommand(const CU::Vector3f& position)
		: RenderCommand(RenderCommand::PARTICLE)
		, m_Position(position)
	{
	}

	CU::Vector3f m_Position;

};

