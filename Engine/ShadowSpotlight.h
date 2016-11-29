#pragma once
#include "snowblind_shared.h"
namespace Snowblind
{
	class Texture;
	class CCamera;
	class ShadowSpotlight
	{
	public:
		ShadowSpotlight() = default;

		bool Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size);
		bool CleanUp();

		CCamera* GetCamera() { return m_Camera; }
		CCamera* GetCamera() const { return m_Camera; }
		void SetViewport();

		Texture* GetTexture() { return m_Texture; }
		Texture* GetRenderTarget() { return m_RenderTarget; }

		void ClearTexture();

		void SetTargets();
		CU::Matrix44f GetOrientation();

		Texture* m_Texture;
		Texture* m_Normal;
		Texture* m_Depth;
		Texture* m_RenderTarget;
		CU::Matrix44f GetMVP();
	private:
		CCamera* m_Camera = nullptr;
		Viewport* m_Viewport = nullptr;
		IDevice* m_Device = nullptr;
		IDevContext* m_Context = nullptr;


		CU::Matrix44f m_Bias;
		CU::Vector3f m_Position;
		CU::Vector3f m_Direction;
		float m_BufferSize = 0.f;
	};
};