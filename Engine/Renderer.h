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

	class Camera;
	class DeferredRenderer;
	class DirectionalLight;
	class CEmitterInstance;
	class CModel;
	class CPointLight;
	class SkySphere;
	class Synchronizer;
	class Texture;
	class CText;
	class CTerrain;
	class LightPass;
	class CSpotLight;
	class ShadowSpotlight;

	class Renderer
	{
	public:
		Renderer() = default;

		bool Initiate(Synchronizer* synchronizer, Camera* camera_3d, Camera* camera_2d);
		bool CleanUp();

		void Render();
		void Add2DCamera(Camera* aCamera);
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

		CU::GrowingArray<CTerrain*> myTerrainArray;

		bool m_RenderWireframe = false;

		Engine*			myEngine			= nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11*			m_API				= nullptr;
#else
		Vulkan*				m_API				= nullptr;
#endif
		bool m_ProcessShadows = false;
		Camera*			myCamera			= nullptr;
		CU::Matrix44f		myPrevFrame;
		CU::Matrix44f		myPrevShadowFrame;

		Camera*			my2DCamera			= nullptr;
		CU::TimeManager*	myTimeManager		= nullptr;
		
		DeferredRenderer*	myDeferredRenderer	= nullptr;

		DirectionalLight*	myDirectionalLight	= nullptr;
		CPointLight*		myPointLight		= nullptr;
		CSpotLight*			mySpotlight			= nullptr;
		ShadowSpotlight*	m_Shadowlight		= nullptr;

		Synchronizer*		mySynchronizer		= nullptr;
		CText*				myText				= nullptr;
		Texture*			myDepthTexture		= nullptr;
		SkySphere*			mySkysphere			= nullptr;
		Sprite*				mySprite			= nullptr;
		Sprite*				myClearColor		= nullptr;

		CLine3D*			my3DLine			= nullptr;


		LightPass			m_LightPass;
		ShadowPass			m_ShadowPass;
	};
}; 