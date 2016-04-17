#pragma once
#include <DataStructures/GrowingArray.h>

namespace Snowblind
{
	class CCamera;
	class CInstance;
	class CSprite;
	class CScene
	{
	public:
		CScene();
		~CScene();

		void Initiate(CCamera* aCamera, bool aIs2DScene = false);
		void Render();
		void Update(float aDeltaTime);
		void AddToScene(CInstance* anInstance);
		void AddToScene(CSprite* aSprite);
	private:
		CCamera* myCamera;
		CU::GrowingArray<CInstance*> myInstances;
		CU::GrowingArray<CSprite*> my2DInstances;

		bool myIs2DScene;
	};
};