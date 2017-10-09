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

	void Render(const graphics::RenderContext& rc) override;
	void ShadowRender(const graphics::RenderContext& rc) override;

	const CU::Vector3f& GetPosition() { return m_Orientation.GetPosition(); }

	void SetupRefractionRender(const graphics::RenderContext& rc);
	void SetupReflectionRender(const graphics::RenderContext& rc);
	void SetClipPlane(const CU::Vector4f& plane, const graphics::RenderContext& rc);
	Effect* GetEffect() { return m_Effect; }
	void AddSurface(Surface* surface) override;
private:
	void UpdateConstantBuffer(const graphics::RenderContext& rc);
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

