#include "stdafx.h"
#include "Scene.h"
#include "Instance.h"
#include "Sprite.h"
#include "Text.h"
namespace Snowblind
{
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
			for (int i = 0; i < myText.Size(); i++)
			{
				myText[i]->Render();
			}
			CEngine::GetDirectX()->EnableZBuffer();
		}
	}

	void CScene::Update(float aDeltaTime)
	{
		for (int i = 0; i < myInstances.Size(); i++)
		{
			myInstances[i]->Update(aDeltaTime);
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

	void CScene::AddToScene(CText* aText)
	{
		myText.Add(aText);
	}

};