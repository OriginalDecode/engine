#pragma once


namespace Snowblind
{
	class CModel;
	class CEffect;
	class CCamera;
	class DirectX11;

	struct SkysphereLayer
	{
		CModel* m_Model;
		CTexture* m_Texture;
		bool m_ShouldRotate = false;
	};

	class CSkySphere
	{
	public:
		CSkySphere() = default;
		bool Initiate(const std::string& model_filepath, const std::string& skysphere_shader, CCamera* camera);
		bool AddLayer(const std::string& layer_filepath, const std::string& layer_shader);
		bool CleanUp();
		void Render(CU::Matrix44f& anOrientation, CTexture* aDepthTexture);
		void SetPosition(const CU::Vector3f& aPosition);
		void Update(float dt);
	private:
		CCamera* myCamera;
#ifdef SNOWBLIND_DX11
		DirectX11* myAPI;
#else
		Vulkan* myAPI;
#endif
		CU::Matrix44f myOrientation;
		CU::GrowingArray<SkysphereLayer> m_Layers;
		
	};
};