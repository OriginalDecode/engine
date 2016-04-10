#include "stdafx.h"
#include "Scene.h"
#include "Instance.h"
namespace Snowblind
{
	CScene::CScene()
	{
	}


	CScene::~CScene()
	{
		myInstances.RemoveAll();
	}

	void CScene::Initiate(CCamera* aCamera)
	{
		myCamera = aCamera;
	}

	void CScene::Render()
	{
		for (int i = 0; i < myInstances.Size(); i++)
		{
			myInstances[i]->Render(*myCamera);
		}
	}

	void CScene::Update(float aDeltaTime)
	{
		for (int i = 0; i < myInstances.Size(); i++)
		{
			myInstances[i]->Update(((90.f / 180.f)*float(3.14f)) * aDeltaTime);
		}

	}

	void CScene::AddToScene(CInstance* anInstance)
	{
		myInstances.Add(anInstance);
	}

};