#pragma once
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
	class CSkySphere;
	class CModel;

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

		CAssetsContainer* myAssetsContainer;
		CEngine* myEngine;
		CDirectX11* myDirectX;

		CCamera* myCamera;
		CU::Matrix44f myPrevFrame;

		CCamera* my2DCamera;
		CU::Vector3f mySpherePos;
		CU::TimeManager* myTimeManager;
		
		CDeferredRenderer* myDeferredRenderer;
		CSynchronizer& mySynchronizer;
		CPointLight* myPointLight;
		CText* myText;
		CTexture* myDepthTexture;
		CSkySphere* mySkysphere;
		CSprite* mySprite;
	};
}; 