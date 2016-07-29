#pragma once


namespace Snowblind
{
	class CModel;
	class CEffect;
	class CCamera;
	class CDirectX11;
	class CSkySphere
	{
	public:
		CSkySphere(const std::string& aFilePath, const std::string& anEffect, CCamera* aCamera);
		~CSkySphere();

		void Render(CU::Matrix44f& anOrientation, CTexture* aDepthTexture);
		void SetPosition(const CU::Vector3f& aPosition);
	private:
		CCamera* myCamera;
		CTexture* mySkysphereTexture;
		CU::Matrix44f myOrientation;
		CDirectX11* myDirectX;
		CModel* myModel;
		
	};
};