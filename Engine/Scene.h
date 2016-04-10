#pragma once
#include <DataStructures/GrowingArray.h>

namespace Snowblind
{
	class CCamera;
	class CInstance;

	class CScene
	{
	public:
		CScene();
		~CScene();

		void Initiate(CCamera* aCamera);
		void Render();
		void Update(float aDeltaTime);
		void AddToScene(CInstance* anInstance);
	private:
		CCamera* myCamera;
		CU::GrowingArray<CInstance*> myInstances;


	};
};