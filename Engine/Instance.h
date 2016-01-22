#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
namespace Snowblind
{
	class CModel;
	class CCamera;
	class CInstance
	{
	public:
		CInstance(CModel* aModel);
		~CInstance();

		void Update(float aRotation);
		void Render(CCamera& aCamera);

	private:

		void operator=(CInstance&) = delete;

		CModel& myModel;
		Matrix44f myOrientation;



	};
};