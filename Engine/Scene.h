//#pragma once
//#include <DataStructures/GrowingArray.h>
//#include <DataStructures/StaticArray.h>
//#include "LightStructs.h"
//
//class Camera;
//class CInstance;
//class Sprite;
//class CText;
//class DirectionalLight;
//class PointLight;
//
//class CScene
//{
//public:
//	CScene();
//	~CScene();
//
//	void Initiate(Camera* aCamera, bool aIs2DScene = false);
//	void Render();
//	void Update(float aDeltaTime);
//
//	void AddToScene(CInstance* anInstance);
//	void AddToScene(Sprite* aSprite);
//	void AddToScene(CText* aText);
//
//	void AddLight(DirectionalLight* aDirectionalLight);
//	void AddLight(PointLight* aPointLight);
//
//
//private:
//
//	void RenderLight();
//	void UpdateLight(float aDeltaTime);
//
//	Camera* myCamera;
//	CU::GrowingArray<CInstance*> myInstances;
//	CU::GrowingArray<Sprite*> my2DInstances;
//	CU::GrowingArray<CText*> myText;
//
//	CU::GrowingArray<DirectionalLight*> myDirectionalLights;
//	CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE> myDirectionalLightData;
//
//	CU::GrowingArray<PointLight*> myPointLights;
//	CU::StaticArray<SPointlightData, POINTLIGHT_SIZE> myPointLightData;
//
//
//	bool myIs2DScene;
//};
