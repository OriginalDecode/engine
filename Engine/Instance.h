#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
namespace Snowblind
{
	class CModel;
	class CCamera;
	class CInstance
	{
	public:
		CInstance();
		~CInstance();
		void Initiate(CModel* aModel);
		void Update(float aRotation);
		void Render(CCamera& aCamera);
		void SetPosition(CU::Math::Vector3<float> aPosition);


	private:

		void operator=(CInstance&) = delete;

		CModel* myModel;
		Matrix44f myOrientation;



	};
};