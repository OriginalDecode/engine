#pragma once
namespace Snowblind
{
	class CTexture;
	class CCamera;
	class ShadowSpotlight
	{
	public:
		ShadowSpotlight() = default;

		bool Initiate(const CU::Vector3f& position, const CU::Vector3f& direction);
		bool CleanUp();

		CCamera* GetCamera() { return m_Camera; }
		CCamera* GetCamera() const { return m_Camera; }
		void SetViewport();
		CTexture* GetTexture() { return m_Texture; }
		void ClearTexture();

	private:

		CTexture* m_Texture;

		CCamera* m_Camera; // Unique for Shadows.
		D3D11_VIEWPORT* m_Viewport;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_Context;
		CU::Vector3f m_Position;
		CU::Vector3f m_Direction;

	};
};