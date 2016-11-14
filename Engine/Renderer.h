#pragma once
#include "LightStructs.h"
#include "ShadowPass.h"
#include "LightPass.h"

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
	class LightPass;
	class CSpotLight;
	class ShadowSpotlight;

	class CRenderer
	{
	public:
		CRenderer() = default;

		bool Initiate(CSynchronizer* synchronizer, CCamera* camera_3d, CCamera* camera_2d);
		bool CleanUp();

		void Render();
		void Add2DCamera(CCamera* aCamera);
		void AddTerrain(CTerrain* someTerrain);
		void ToggleWireframe();
	private:

		void Render3DCommands();
		void Render2DCommands();
		void RenderPointlight();
		void RenderSpotlight();

		void RenderParticles();
		void RenderLines();
		void ProcessShadows();

		//Holds the lightpass instead of deferred

		CU::GrowingArray<CTerrain*> myTerrainArray;

		bool m_RenderWireframe = false;

		CEngine*			myEngine			= nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11*			myDirectX			= nullptr;
#endif
		CCamera*			myCamera			= nullptr;
		CU::Matrix44f		myPrevFrame;

		CCamera*			my2DCamera			= nullptr;
		CU::TimeManager*	myTimeManager		= nullptr;
		
		CDeferredRenderer*	myDeferredRenderer	= nullptr;

		CDirectionalLight*	myDirectionalLight	= nullptr;
		CPointLight*		myPointLight		= nullptr;
		CSpotLight*			mySpotlight			= nullptr;
		ShadowSpotlight*	m_Shadowlight		= nullptr;

		CSynchronizer*		mySynchronizer		= nullptr;
		CText*				myText				= nullptr;
		CTexture*			myDepthTexture		= nullptr;
		CSkySphere*			mySkysphere			= nullptr;
		CSprite*			mySprite			= nullptr;
		CLine3D*			my3DLine			= nullptr;

		LightPass			m_LightPass;
		ShadowPass			m_ShadowPass;
	};
}; 