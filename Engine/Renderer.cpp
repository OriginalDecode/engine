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
#include "EmitterInstance.h"

namespace Snowblind
{

	CRenderer::CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera)
		: mySynchronizer(aSynchronizer)
		, myCamera(aCamera)
	{
		myText = new CText("Data/Font/OpenSans-Bold.ttf", 15); //bug cant go below 15
		myPointLight = new CPointLight();
		myDeferredRenderer = new CDeferredRenderer();
		myDepthTexture = new CTexture();
		myDepthTexture->InitAsDepthBuffer(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight);
	}

	CRenderer::~CRenderer()
	{
		SAFE_DELETE(myDepthTexture);
		SAFE_DELETE(my2DCamera);
		SAFE_DELETE(myDeferredRenderer);
		SAFE_DELETE(myText);
		SAFE_DELETE(myPointLight);
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{
		CEngine::Clear();

		myDeferredRenderer->SetTargets();
		Render3DCommands();
		myDepthTexture->CopyData(myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->SetBuffers();
		myDeferredRenderer->DeferredRender();

		myDeferredRenderer->SetLightState(myCamera);
		RenderLightCommands();
		myDeferredRenderer->SetNormalState();

		RenderParticles();

		Render2DCommands();


		CEngine::Present();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
	}

	void CRenderer::Render3DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::e3D);
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
		const CU::GrowingArray<SRenderCommand>& commands2D = mySynchronizer.GetRenderCommands(eCommandType::e2D);
		CEngine::GetDirectX()->DisableZBuffer();
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
		CEngine::GetDirectX()->EnableZBuffer();

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

	void CRenderer::RenderParticles()
	{
		CEngine::GetDirectX()->SetBlendState(eBlendStates::ALPHA_BLEND);
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_NONE);
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::PARTICLE);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::PARTICLE:
				command.myEmitterInstance->Render(myCamera, myDepthTexture);
				break;
			}
		}
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
	}
};