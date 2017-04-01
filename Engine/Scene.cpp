#include "stdafx.h"
#include "Scene.h"
#include "Instance.h"
#include "Sprite.h"
#include "Text.h"
#include "DirectionalLight.h"
#include "PointLight.h"

CScene::CScene()
{
}


CScene::~CScene()
{
	delete myCamera;
	myCamera = nullptr;
	myInstances.DeleteAll();
	my2DInstances.DeleteAll();
	myText.DeleteAll();
	myDirectionalLights.DeleteAll();
	myPointLights.DeleteAll();
}

void CScene::Initiate(Camera* aCamera, bool aIs2DScene)
{
	myCamera = aCamera;
	myIs2DScene = aIs2DScene;
}

void CScene::Render()
{
#ifdef SNOWBLIND_DX11
	if (!myIs2DScene)
	{
		RenderLight();
		for (int i = 0; i < myInstances.Size(); i++)
		{
			//myInstances[i]->UpdateLight(myDirectionalLightData);
			//myInstances[i]->UpdateLight(myPointLightData);
//				myInstances[i]->Render(*myCamera);
		}
	}
	else
	{
		Engine::DisableZ();
		for (int i = 0; i < my2DInstances.Size(); i++)
		{
			my2DInstances[i]->Render(myCamera);
		}
		for (int i = 0; i < myText.Size(); i++)
		{
			myText[i]->Render(myCamera);
		}
		Engine::EnableZ();
	}
#endif
}

void CScene::Update(float aDeltaTime)
{
	UpdateLight(aDeltaTime);
	for (int i = 0; i < myInstances.Size(); i++)
	{
		//myInstances[i]->Update(aDeltaTime);
	}
}

void CScene::AddToScene(CInstance* anInstance)
{
	myInstances.Add(anInstance);
}

void CScene::AddToScene(Sprite* aSprite)
{
	my2DInstances.Add(aSprite);
}

void CScene::AddToScene(CText* aText)
{
	myText.Add(aText);
}

void CScene::AddLight(DirectionalLight* aDirectionalLight)
{
#ifdef _DEBUG
	//myInstances->Add(aDirectionalLight->GetInstance());
#endif
	myDirectionalLights.Add(aDirectionalLight);
}

void CScene::AddLight(CPointLight* aPointLight)
{
	myPointLights.Add(aPointLight);
}

void CScene::RenderLight()
{
	for (int i = 0; i < myDirectionalLights.Size(); i++)
	{
		DirectionalLight* dirLight = myDirectionalLights[i];

		myDirectionalLightData[i].myLightColor = dirLight->GetColor();
		myDirectionalLightData[i].myLightDirection = dirLight->GetDirection();
	}

	for (int i = 0; i < myPointLights.Size(); i++)
	{
		CPointLight* pointLight = myPointLights[i];

		myPointLightData[i].myRange = pointLight->GetRange();
		myPointLightData[i].myLightColor = pointLight->GetColor();
		myPointLightData[i].myLightPosition = pointLight->GetPosition();
	}

}

void CScene::UpdateLight(float aDeltaTime)
{
	for (int i = 0; i < myDirectionalLights.Size(); i++)
	{
		DirectionalLight* dirLight = myDirectionalLights[i];
		dirLight->Rotate(eLightAxis::ZAxis, CL::DegreeToRad(45.f) * aDeltaTime);
		dirLight->Update();
	}
}

