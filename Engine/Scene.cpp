#include "stdafx.h"
#include "Scene.h"
#include "Instance.h"
#include "Sprite.h"
namespace Snowblind
{
	CScene::CScene()
	{
	}


	CScene::~CScene()
	{
		myInstances.RemoveAll();
	}

	void CScene::Initiate(CCamera* aCamera, bool aIs2DScene)
	{
		myCamera = aCamera;
		myIs2DScene = aIs2DScene;
	}

	void CScene::Render()
	{
		if (!myIs2DScene)
		{
			for (int i = 0; i < myInstances.Size(); i++)
			{
				myInstances[i]->Render(*myCamera);
			}
		}
		else
		{
			CEngine::GetDirectX()->DisableZBuffer();
			for (int i = 0; i < my2DInstances.Size(); i++)
			{
				my2DInstances[i]->Render(myCamera);
			}
			CEngine::GetDirectX()->EnableZBuffer();
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

	void CScene::AddToScene(CSprite* aSprite)
	{
		my2DInstances.Add(aSprite);
	}

};