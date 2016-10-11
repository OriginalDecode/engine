#pragma once
#include "LightStructs.h"

namespace CommonUtilities
{
	class TimeManager;
}


class CLine3D;

namespace Snowblind
{

	class CCamera;
	class CDeferredRenderer;
	class CDirectionalLight;
	class CEmitterInstance;
	class CModel;
	class CPointLight;
	class CSkySphere;
	class CSynchronizer;
	class CTexture;
	class CText;
	class CTerrain;
	class CLightPass;
	class CSpotLight;
	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();
		void AddTerrain(CTerrain* someTerrain);
	private:
		void Render3DCommands();
		void Render2DCommands();
		void RenderPointlight();
		void RenderSpotlight();

		void RenderParticles();
		void RenderLines();

		//Holds the lightpass instead of deferred

		CU::GrowingArray<CTerrain*> myTerrainArray;

		eDeferredType		myDeferredType;
		CEngine*			myEngine = nullptr;
#ifndef SNOWBLIND_VULKAN
		DirectX11*			myDirectX = nullptr;
#endif
		CCamera*			myCamera = nullptr;
		CU::Matrix44f		myPrevFrame;

		CCamera*			my2DCamera = nullptr;
		CU::TimeManager*	myTimeManager = nullptr;
		
		CDeferredRenderer*	myDeferredRenderer = nullptr;

		CDirectionalLight*	myDirectionalLight = nullptr;
		CPointLight*		myPointLight = nullptr;
		CSpotLight*			mySpotlight = nullptr;

		CSynchronizer&		mySynchronizer;
		CText*				myText = nullptr;
		CTexture*			myDepthTexture = nullptr;
		CSkySphere*			mySkysphere = nullptr;
		CSprite*			mySprite = nullptr;
		CLine3D*			my3DLine = nullptr;
		CLightPass*			myLightPass = nullptr;
	};
}; 