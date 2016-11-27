#pragma once
namespace Snowblind
{
	class CTexture;
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

		CTexture* GetTexture() { return m_Texture; }
		CTexture* GetRenderTarget() { return m_RenderTarget; }

		void ClearTexture();

		void SetTargets();
		CU::Matrix44f GetOrientation();

		CTexture* m_Texture;
		CTexture* m_Normal;
		CTexture* m_Depth;
		CTexture* m_RenderTarget;
	private:

		CU::Matrix44f m_Orientation;

		CCamera* m_Camera; // Unique for Shadows.
		D3D11_VIEWPORT* m_Viewport;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_Context;
		CU::Vector3f m_Position;
		CU::Vector3f m_Direction;
		float m_BufferSize = 0.f;
	};
};