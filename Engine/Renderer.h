#pragma once
#include <DataStructures/StaticArray.h>

#include "LightStructs.h"


namespace CommonUtilities
{
	class TimeManager;
}


namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CDeferredRenderer;
	class CCamera;
	class CPointLight;
	class CEmitterInstance;
	class CTexture;

	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();
	private:
		eDeferredType myDeferredType;
		void Render3DCommands();
		void Render2DCommands();
		void RenderLightCommands();
		void RenderParticles();

		CU::TimeManager* myTimeManager;
		CDeferredRenderer* myDeferredRenderer;
		CCamera* myCamera;
		CCamera* my2DCamera;
		CSynchronizer& mySynchronizer;
		CPointLight* myPointLight;
		CText* myText;

		CTexture* myDepthTexture;
	
	};
}; 