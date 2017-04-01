#pragma once
#include "snowblind_shared.h"

	class Texture;
	class Camera;
	class Effect;

	class ShadowSpotlight
	{
	public:
		ShadowSpotlight() = default;

		bool Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size);
		bool CleanUp();

		Camera* GetCamera() { return m_Camera; }
		Camera* GetCamera() const { return m_Camera; }
		void SetViewport();

		Texture* GetDepthStencil() { return m_DepthStencil; }
		Texture* GetDepthTexture() { return m_Depth; }

		void ClearTexture();
		void SetTargets();

		void ToggleShader(bool on_or_off);

		void SetOrientation(const CU::Matrix44f& orientation);


		CU::Matrix44f GetOrientation();
		CU::Matrix44f GetMVP();
		void Copy();
	private:
		Effect* m_ShadowEffect = nullptr;
		Camera* m_Camera = nullptr;
		Viewport* m_Viewport = nullptr;
		IDevice* m_Device = nullptr;
		IDevContext* m_Context = nullptr;

		Texture* m_Depth;
		Texture* m_DepthStencil;
		Texture* m_Holder;


		CU::Vector3f m_Position;
		CU::Vector3f m_Direction;
		float m_BufferSize = 0.f;

	};
