#pragma once
#include "snowblind_shared.h"
namespace Snowblind
{
	class Texture;
	class CCamera;
	class Effect;

	class ShadowSpotlight
	{
	public:
		ShadowSpotlight() = default;

		bool Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size);
		bool CleanUp();

		CCamera* GetCamera() { return m_Camera; }
		CCamera* GetCamera() const { return m_Camera; }
		void SetViewport();

		Texture* GetDepthStencil() { return m_DepthStencil; }
		Texture* GetDepthTexture() { return m_Depth; }

		void ClearTexture();
		void SetTargets();

		void ToggleShader(bool on_or_off);

		CU::Matrix44f GetOrientation();
		CU::Matrix44f GetMVP();
	private:
		Effect* m_ShadowEffect = nullptr;
		CCamera* m_Camera = nullptr;
		Viewport* m_Viewport = nullptr;
		IDevice* m_Device = nullptr;
		IDevContext* m_Context = nullptr;

		Texture* m_Depth;
		Texture* m_DepthStencil;

		CU::Matrix44f m_Bias;
		CU::Vector3f m_Position;
		CU::Vector3f m_Direction;
		float m_BufferSize = 0.f;
	};
};