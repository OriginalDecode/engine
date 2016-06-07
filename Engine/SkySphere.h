#pragma once


namespace Snowblind
{
	class CModel;
	class CEffect;
	class CCamera;
	class CSkySphere
	{
	public:
		CSkySphere(const std::string& aFilePath, const std::string& anEffect, CCamera* aCamera);
		~CSkySphere();

		void Render(CU::Matrix44f& anOrientation);
		void SetPosition(const CU::Vector3f& aPosition);
	private:
		CCamera* myCamera;
		CTexture* mySkysphereTexture;
		CU::Matrix44f myOrientation;
		CU::Matrix44f myPrevOrientation;

		CModel* myModel;
		
	};
};