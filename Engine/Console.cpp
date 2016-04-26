#include "stdafx.h"
#include "Console.h"
#include "Sprite.h"
#include "TextureContainer.h"
#include "Engine.h"
namespace Snowblind
{
	CConsole::CConsole()
	{

	}


	CConsole::~CConsole()
	{

	}

	void CConsole::Initiate(CCamera* aCamera)
	{
		myCamera = aCamera;
		myIsActive = false;

		mySprite = new CSprite();
		mySprite->Initiate("Data/Textures/colors.dds", { 1920.f / 2.f, 1080.f / 2.f}, { 0, 0 });
		mySprite->SetHotspot({ -1920.f / 4.f, 1080.f / 4.f });
		mySprite->SetPosition({ CEngine::GetInstance()->GetWindowSize().myWidth, 0 });
	}

	void CConsole::Render()
	{
		if (myIsActive)
		{
			CEngine::GetDirectX()->DisableZBuffer();
			mySprite->Render(myCamera);
			CEngine::GetDirectX()->EnableZBuffer();
		}
	}

	void CConsole::Update()
	{
		if (myIsActive)
		{

		}
	}

	void CConsole::ToggleConsole()
	{
		myIsActive = !myIsActive;
	}
};