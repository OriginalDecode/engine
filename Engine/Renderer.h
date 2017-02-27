#pragma once
#include "LightStructs.h"
#include "ShadowPass.h"
#include "LightPass.h"
#include "RenderCommand.h"
#include <Math/Matrix/Matrix.h>

namespace CommonUtilities
{
	class TimeManager;
}

class CLine3D;

namespace Hex
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

		bool Initiate(Synchronizer* synchronizer, Camera* camera);
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
		void RenderDirectionalLight();

		void RenderParticles();
		void RenderLines();
		//Only works for the spotlight
		void ProcessShadows();

		void ProcessShadows(Camera* camera);

		CU::GrowingArray<CTerrain*> myTerrainArray;

		bool				m_RenderWireframe	= false;
		bool				m_ProcessShadows	= false;

		Engine*				m_Engine			= nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11*			m_API				= nullptr;
#else
		Vulkan*				m_API				= nullptr;
#endif
		Camera*				m_Camera			= nullptr;
		CU::Matrix44f		myPrevFrame;
		CU::Matrix44f		myPrevShadowFrame;
		CU::Matrix44f		m_DirectionalFrame;
		bool m_ProcessDirectionalShadows = false;
		Camera*				my2DCamera			= nullptr;
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

		CEmitterInstance*	m_ParticleEmitter	= nullptr;

		LightPass			m_LightPass;
		ShadowPass			m_ShadowPass;
		Texture*			m_LightTexture		= nullptr;
		Effect* m_DirectionalLight = nullptr;
		Effect* m_ToneMapping = nullptr;


		IBuffer* m_DirectionalLightBuffer = nullptr;

		CU::Vector3f m_Direction;
		CU::Vector3f m_OriginalDirection;
		CU::Math::Matrix33<float> m_Orientation;

		Camera* m_DirectionalCamera = nullptr;
		Effect* m_ShadowEffect = nullptr;
		Texture* m_ShadowDepth = nullptr;
		Texture* m_ShadowDepthStencil = nullptr;

		struct cbDirectionalLightPS
		{
			CU::Matrix44f m_InvertedProjection;
			CU::Matrix44f m_View;
			CU::Vector4f m_Color;
			CU::Vector4f m_CameraPosition;
			CU::Matrix44f m_ShadowMVP;
			CU::Vector4f m_Direction;
		} m_DirectionalLightStruct;


		RenderCommand		m_CubeCommand;
	};
}; 