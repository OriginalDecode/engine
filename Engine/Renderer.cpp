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
	}

	CRenderer::~CRenderer()
	{
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
				command.myInstance->Render(myCamera);
				break;
			case SRenderCommand::eType::POINTLIGHT:
				//command.myPointLight->
				break;
			}
		}

		CEngine::Present();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
	}
};