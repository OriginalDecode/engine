#pragma once


namespace Snowblind
{
	class CModel;
	class CEffect;

	class CSkySphere
	{
	public:
		CSkySphere(const std::string& aFilePath, const std::string& anEffect);
		~CSkySphere();

		void Render(CCamera* aCamera);
		void SetPosition(const CU::Vector3f& aPosition);
	private:

		CTexture* mySkysphereTexture;
		CU::Matrix44f myOrientation;
		CModel* myModel;
		
	};
};