#pragma once
#include <DataStructures/GrowingArray.h>
#include <DataStructures/StaticArray.h>
#include "LightStructs.h"
namespace Snowblind
{
	class Camera;
	class CInstance;
	class Sprite;
	class CText;
	class CDirectionalLight;
	class CPointLight;

	class CScene
	{
	public:
		CScene();
		~CScene();

		void Initiate(Camera* aCamera, bool aIs2DScene = false);
		void Render();
		void Update(float aDeltaTime);

		void AddToScene(CInstance* anInstance);
		void AddToScene(Sprite* aSprite);
		void AddToScene(CText* aText);

		void AddLight(CDirectionalLight* aDirectionalLight);
		void AddLight(CPointLight* aPointLight);


	private:

		void RenderLight();
		void UpdateLight(float aDeltaTime);

		Camera* myCamera;
		CU::GrowingArray<CInstance*> myInstances;
		CU::GrowingArray<Sprite*> my2DInstances;
		CU::GrowingArray<CText*> myText;

		CU::GrowingArray<CDirectionalLight*> myDirectionalLights;
		CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE> myDirectionalLightData;

		CU::GrowingArray<CPointLight*> myPointLights;
		CU::StaticArray<SPointlightData, POINTLIGHT_SIZE> myPointLightData;


		bool myIs2DScene;
	};
};