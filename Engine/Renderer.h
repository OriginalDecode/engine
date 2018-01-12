#pragma once
#include <Engine/engine_shared.h>
#include <CommonLib/Math/Matrix/Matrix33.h>
#include <Math/Matrix/Matrix.h>


#include "LightStructs.h"
#include "ShadowPass.h"
#include "RenderCommand.h"
#include "PostProcessManager.h"

#include <Engine/Atmosphere.h>
#include <Engine/Shadow_Directional.h>
#include <Engine/ShaderState.h>
#include <Engine/GBuffer.h>
#include <Engine/RenderContext.h>
#include <Engine/LightPass.h>
#include <Engine/InstancingManager.h>
#include <CommonLib/RefPointer.h>


class Line3D;
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
class SpotLight;
class ShadowSpotlight;
class CommandAllocator;
class Renderer
{
#if !defined(_PROFILE) && !defined(_FINAL)
	friend debug::DebugHandle;
#endif
public:
	Renderer(Synchronizer* synchronizer);
	~Renderer();


	void Render();


	void ProcessWater();

	void AddTerrain(Terrain* someTerrain);

	void Render3DShadows(const CU::Matrix44f& orientation, Camera* camera);

	int RegisterLight();
	void SetDirection(const CU::Vector3f& dir) { m_Direction = dir; }
	const CU::Vector3f& GetLightDirection() { return m_Direction; }
	Camera* GetDirectionalCamera() { return m_DirectionalShadow.GetCamera(); }


private:
	void RenderNonDeferred3DCommands();
	void Render3DCommands();

	void Render3DCommandsInstanced();

	void ProcessModelCommand(const memory::CommandAllocator& commands, s32 i, Engine& engine);

	void RenderTerrain(bool override_effect);

	void Render2DCommands();

	void RenderPointlight();
	void RenderSpotlight();

	void RenderParticles(Effect* effect);
	void RenderLines();



	//instancing helper?
	CU::GrowingArray<Model*> m_Models;
	std::map<u64, Model*> m_ModelsToRender;

	CU::GrowingArray<Terrain*> myTerrainArray;

	CU::Vector3f		m_Direction;
	CU::Vector3f		m_OriginalDirection;
	CU::Matrix33f		m_Orientation;

	graphics::GBuffer m_GBuffer;
	graphics::RenderContext		m_RenderContext;

	graphics::LightPass*		m_LightPass = nullptr;
	ShadowPass					m_ShadowPass;

	PostProcessManager			m_PostProcessManager;

	Atmosphere					m_Atmosphere;


	InstancingManager m_InstancingManager;

	//Do we need to hold these?
	Camera*						m_Camera				= nullptr;
	Camera*						m_WaterCamera			= nullptr;

	//Does this have to be a pointer?
	DeferredRenderer*			m_DeferredRenderer		= nullptr;

	// move to a light manager
	DirectionalLight*			myDirectionalLight		= nullptr;
	PointLight*					myPointLight			= nullptr;
	SpotLight*					mySpotlight				= nullptr;
	CU::GrowingArray<SpotLight*> m_Spotlights; //This should be in a light manager


	Synchronizer*				m_Synchronizer			= nullptr;
	//CText*						myText					= nullptr;
	Texture*					m_DepthTexture			= nullptr;

	Line3D*					m_Line					= nullptr;

	CEmitterInstance*			m_ParticleEmitter		= nullptr;

	//How do we handle this?
	ShadowDirectional			m_DirectionalShadow;


	IBuffer*					m_ViewProjBuffer = nullptr; 

	//this should be an object in the world and the render part should be separated
	class WaterPlane*			m_WaterPlane				= nullptr; //Shouldn't be in here

	bool m_RenderInstanced = true;

	IBuffer* m_PerFramePixelBuffer = nullptr;
	struct PerFramePixelBuffer
	{
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
		CU::Vector4f m_CameraPos;
	} m_PerFramePixelStruct;


#if !defined(_PROFILE) && !defined(_FINAL)
	bool m_RenderLines = false;
	bool m_LightModelWireframe = false;


	Texture* m_DebugTexture = nullptr;
	//CU::GrowingArray<Texture*> m_DebugTextures;
	Texture* m_HoverTexture = nullptr;
	Texture* m_SelectedTexture = nullptr;
	RefPointer<Model> m_HoverModel = nullptr;
	RefPointer<Model> m_SelectedModel = nullptr;

	Effect* m_RenderHoverEffect = nullptr;
	Effect* m_SelectedEffect = nullptr;
	Quad* m_DebugQuad = nullptr;
	void WriteDebugTextures();

	void DrawEntity(Texture* pTex, Entity e, graphics::IGraphicsContext &ctx);

public:
	void SetRenderLines(bool render_lines) { m_RenderLines = render_lines; }
	bool GetRenderLines() { return m_RenderLines; }
	DeferredRenderer* GetDeferredRenderer() {	return m_DeferredRenderer; };


#endif


};
