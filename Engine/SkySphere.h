#pragma once


namespace Snowblind
{
	class CModel;
	class CEffect;

	class CSkySphere
	{
	public:
		CSkySphere(const CCamera& aCamera);
		~CSkySphere();
		void Update(float aDeltaTime);
		void Render();

	private:

		const CCamera& myCamera;
		CU::Matrix44f myOrientation;
		CModel* myModel;
		
	};
};