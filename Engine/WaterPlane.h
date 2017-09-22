#pragma once
#include <Engine/BaseModel.h>
#include <Engine/GBuffer.h>
class Texture;
class WaterPlane : public BaseModel
{
public:
	WaterPlane();
	~WaterPlane();

	void SetPosition(const CU::Vector3f& position);

	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;

	const CU::Vector3f& GetPosition() { return m_Orientation.GetPosition(); }

	void SetupRefractionRender(const graphics::RenderContext& render_context);
	void SetupReflectionRender(const graphics::RenderContext& render_context);
	void SetClipPlane(const CU::Vector4f& plane, const graphics::RenderContext& render_context);
	Effect* GetEffect() { return m_Effect; }
private:
	void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context);
	void CreatePlane();

	struct cbMatrices
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
		CU::Vector3f m_CameraPos;
		float m_Time;
	} m_VertexMatrices;

	CU::Matrix44f m_Orientation;

	Texture* m_Refraction = nullptr;
	Texture* m_Reflection = nullptr;

	graphics::GBuffer m_RefractionG;
	graphics::GBuffer m_ReflectionG;

	IBuffer* m_cbPixel = nullptr;
	struct cbPixel
	{
		CU::Vector4f m_CompareValue;
	} m_PixelStruct;

	bool m_RenderWireframe = false;
};

