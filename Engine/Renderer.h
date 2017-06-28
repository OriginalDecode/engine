#pragma once
#include <Engine/engine_shared.h>
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
class Terrain;
class LightPass;
class SpotLight;
class ShadowSpotlight;
class CommandAllocator;
class Renderer
{
public:
	Renderer() = default;

	bool Initiate(Synchronizer* synchronizer, Camera* camera);
	bool CleanUp();

	void Render();
	void AddTerrain(Terrain* someTerrain);

	PostProcessManager& GetPostprocessManager();

	void Render3DShadows(const CU::Matrix44f& orientation, Camera* camera);

	int RegisterLight();

private:
	void RenderNonDeferred3DCommands();
	void Render3DCommands();

	void Render3DCommandsInstanced();

	void ProcessCommand(const memory::CommandAllocator& commands, s32 i);

	void RenderTerrain(bool override_effect);

	void Render2DCommands();

	void RenderPointlight();
	void RenderSpotlight();

	void RenderParticles();
	void RenderLines();

	CU::GrowingArray<Model*> m_Models;
	std::map<std::string, Model*> m_ModelsToRender;
	CU::GrowingArray<Terrain*> myTerrainArray;

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
	Camera*				m_WaterCamera			= nullptr;

	DeferredRenderer*	m_DeferredRenderer		= nullptr;

	DirectionalLight*	myDirectionalLight		= nullptr;
	PointLight*			myPointLight			= nullptr;
	SpotLight*			mySpotlight				= nullptr;


	Synchronizer*		mySynchronizer			= nullptr;
	CText*				myText					= nullptr;
	Texture*			myDepthTexture			= nullptr;
	Sprite*				mySprite				= nullptr;
	Sprite*				myClearColor			= nullptr;

	CLine3D*			m_Line					= nullptr;

	CEmitterInstance*	m_ParticleEmitter		= nullptr;

	ShadowDirectional	m_DirectionalShadow;

	class WaterPlane* m_WaterPlane = nullptr;

	ShaderState m_LightState;

	float m_SpriteWidth = 0.f;
	float m_SpriteHeight = 0.f;


#if !defined(_PROFILE) && !defined(_FINAL)
	bool m_RenderLines = false;

public:
	void SetRenderLines(bool render_lines) { m_RenderLines = render_lines; }
	bool GetRenderLines() { return m_RenderLines; }
	DeferredRenderer* GetDeferredRenderer() {	return m_DeferredRenderer; };

#endif
};
