#pragma once
#include <Engine/BaseModel.h>
#include <Engine/GBuffer.h>
class Texture;
class WaterPlane : public BaseModel
{
public:
	WaterPlane() = default;

	void Initiate(const CU::Vector3f& position);

	void SetPosition(const CU::Vector3f& position);

	void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context);
	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;

	const CU::Vector3f& GetPosition() { return m_Orientation.GetPosition(); }

	void SetupRefractionRender(const RenderContext& render_context);
	void SetupReflectionRender(const RenderContext& render_context);
	void SetClipPlane(const CU::Vector4f& plane, const RenderContext& render_context);
	Effect* GetEffect() { return myEffect; }
private:
	void CreatePlane();
	void InitConstantBuffer() override;

	struct cbMatrices
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_InvertedView;
		CU::Matrix44f m_Projection;
		CU::Vector3f m_CameraPos;
		float m_Time;
	} m_VertexMatrices;

	CU::Matrix44f m_Orientation;
	CU::GrowingArray<SVertexPosNormUVBiTang> m_Vertices;
	CU::GrowingArray<int> m_Indices;

	float m_ClearColor[4] = { 0, 0, 0, 0 };
	Texture* m_Refraction = nullptr;
	Texture* m_Reflection = nullptr;

	GBuffer m_RefractionG;
	GBuffer m_ReflectionG;

	IBuffer* m_cbPixel = nullptr;
	struct cbPixel
	{
		CU::Vector4f m_CompareValue;
	} m_PixelStruct;


#if !defined(_PROFILE) && !defined(_FINAL)

	bool m_RenderWireframe = false;
#endif
};

