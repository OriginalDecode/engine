#include "stdafx.h"
#include <thread>

#include "Renderer.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Instance.h"
#include "Camera.h"

namespace Snowblind
{

	CRenderer::CRenderer(CSynchronizer& aSynchronizer, CCamera& aCamera)
		: mySynchronizer(aSynchronizer)
		, myCamera(aCamera)
	{
		myText = new Snowblind::CText("Data/Font/OpenSans-Bold.ttf", 16);
	}

	CRenderer::~CRenderer()
	{
		SAFE_DELETE(my2DCamera);
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{
		CEngine::Clear();

		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands();
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::MODEL:
				command.myInstance->SetPosition(command.myPosition);
				command.myInstance->Render(myCamera);
				break;
			case SRenderCommand::eType::POINTLIGHT:
				//command.myPointLight->
				break;
			case SRenderCommand::eType::TEXT:
				myText->SetText(command.myTextToPrint);
				myText->SetPosition({ command.myPosition.x,command.myPosition.y });
				myText->Render(my2DCamera);
				break;
			}
		}

		CEngine::Present();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
	}
};