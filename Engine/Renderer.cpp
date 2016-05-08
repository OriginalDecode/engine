#include "stdafx.h"
#include <thread>

#include "Renderer.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Instance.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "PointLight.h"

namespace Snowblind
{

	CRenderer::CRenderer(CSynchronizer& aSynchronizer)
		: mySynchronizer(aSynchronizer)
		, myQuitFlag(false)
	{
	}

	CRenderer::~CRenderer()
	{
		SAFE_DELETE(my2DCamera);
		SAFE_DELETE(myDeferredRenderer);
		SAFE_DELETE(myText);
		SAFE_DELETE(myPointLight);
	}

	void CRenderer::RenderMain(float aWidth, float aHeight, HWND* aHWND)
	{
		Initiate(aWidth, aHeight, *aHWND);
		Render();
	}

	void CRenderer::Initiate(float aWidth, float aHeight, HWND aHWND)
	{
		myDirectX = new CDirectX11(aHWND, aWidth, aHeight);

		myText = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 16);
		myPointLight = new Snowblind::CPointLight();
		myDeferredRenderer = new CDeferredRenderer();


	}

	void CRenderer::AddCamera(CCamera* aCamera)
	{
		myCamera = aCamera;
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	CDirectX11* CRenderer::GetDirectX()
	{
		return myDirectX;
	}

	void CRenderer::Render()
	{

		while (mySynchronizer.HasQuit() == false)
		{
			CEngine::Clear();

			myDeferredRenderer->SetTargets();
			Render3DCommands();
			myDeferredRenderer->SetBuffers();
			myDeferredRenderer->DeferredRender();

			myDeferredRenderer->SetLightState(myCamera);
			RenderLightCommands();
			myDeferredRenderer->SetNormalState();

			Render2DCommands();

			CEngine::Present();

			mySynchronizer.WaitForLogic();
			mySynchronizer.SwapBuffer();
			mySynchronizer.RenderIsDone();
		}

		myQuitFlag = true;
	}

	void CRenderer::Render3DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands();
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::MODEL:
				command.myInstance->SetPosition(command.myPosition);
				command.myInstance->Render(*myCamera);
				break;
			}
		}
	}

	void CRenderer::Render2DCommands()
	{
		/* This happens after the deferred pass */
		const CU::GrowingArray<SRenderCommand>& commands2D = mySynchronizer.GetRenderCommands(eCommandType::e2D);
		for each(const SRenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::TEXT:
				myText->SetText(command.myTextToPrint);
				myText->SetPosition({ command.myPosition.x, command.myPosition.y });
				myText->Render(my2DCamera);
				break;
			}
		}
	}

	void CRenderer::RenderLightCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::LIGHT);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::POINTLIGHT:
				myPointLight->SetPosition(command.myPosition);
				myPointLight->SetRange(command.myRange);
				myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, command.myIntensity));
				myPointLight->Update();
				myDeferredRenderer->RenderLight(myPointLight, myCamera);
				break;
			}
		}
	}
};