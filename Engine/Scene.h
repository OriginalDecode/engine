#pragma once
#include <DataStructures/GrowingArray.h>
namespace Snowblind
{
	class CCamera;
	class CInstance;
	class CSprite;
	class CText;
	class CDirectionalLight;


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
		void AddToScene(CText* aText);

		void AddLight(CDirectionalLight* aDirectionalLight);



	private:

		void UpdateLight();

		CCamera* myCamera;
		CU::GrowingArray<CInstance*> myInstances;
		CU::GrowingArray<CSprite*> my2DInstances;
		CU::GrowingArray<CText*> myText;

		CU::GrowingArray<CDirectionalLight*> myDirectionalLights;
		CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE> myDirectionalLightData;


		bool myIs2DScene;
	};
};