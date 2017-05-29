#pragma once
#include <Engine/snowblind_shared.h>
#include "LightStructs.h"
#include "ShadowPass.h"
#include "LightPass.h"
#include "RenderCommand.h"
#include <Math/Matrix/Matrix.h>
#include "PostProcessManager.h"
#include <Engine/Atmosphere.h>
#include <CommonLib/Math/Matrix/Matrix33.h>
#include <Engine/Shadow_Directional.h>
#include <Engine/ShaderState.h>

class CLine3D;

class Camera;
class DeferredRenderer;
class DirectionalLight;
class CEmitterInstance;
class Model;
class PointLight;
class Synchronizer;
class Texture;
class CText;
class CTerrain;
class LightPass;
class SpotLight;
class ShadowSpotlight;

class Renderer
{
public:
	Renderer() = default;

	bool Initiate(Synchronizer* synchronizer, Camera* camera);
	bool CleanUp();

	void Render();
	void AddTerrain(CTerrain* someTerrain);

	PostProcessManager& GetPostprocessManager();

	void Render3DShadows(const CU::Matrix44f& orientation, Camera* camera);

	int RegisterLight();

private:
	void RenderNonDeferred3DCommands();
	void Render3DCommands();
	void Render2DCommands();

	void RenderPointlight();
	void RenderSpotlight();

	void RenderParticles();
	void RenderLines();

	CU::GrowingArray<CTerrain*> myTerrainArray;

	bool				m_ProcessShadows = false;
	bool				m_ProcessDirectionalShadows = false;

	//CU::Matrix44f		myPrevFrame;
	//CU::Matrix44f		myPrevShadowFrame;
	//CU::Matrix44f		m_DirectionalFrame;

	CU::Vector3f		m_Direction;
	CU::Vector3f		m_OriginalDirection;
	CU::Matrix33f		m_Orientation;

	CU::GrowingArray<SpotLight*> m_Spotlights;

	PostProcessManager	m_PostProcessManager;
	LightPass			m_LightPass;
	ShadowPass			m_ShadowPass;
	Atmosphere			m_Atmosphere;

	RenderContext		m_RenderContext;

	Engine*				m_Engine				= nullptr;
	DirectX11*			m_API					= nullptr;
	Camera*				m_Camera				= nullptr;

	DeferredRenderer*	myDeferredRenderer		= nullptr;

	DirectionalLight*	myDirectionalLight		= nullptr;
	PointLight*			myPointLight			= nullptr;
	SpotLight*			mySpotlight				= nullptr;


	Synchronizer*		mySynchronizer			= nullptr;
	CText*				myText					= nullptr;
	Texture*			myDepthTexture			= nullptr;
	Sprite*				mySprite				= nullptr;
	Sprite*				myClearColor			= nullptr;

	CLine3D*			my3DLine				= nullptr;

	CEmitterInstance*	m_ParticleEmitter		= nullptr;

	ShadowDirectional	m_DirectionalShadow;

	ShaderState m_LightState;


};
