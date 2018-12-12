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
#include <Engine/ConstantBuffer.h>

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
class TerrainSystem;

namespace graphics
{
	class IRenderNode;
};

class Renderer
{
#if !defined(_PROFILE) && !defined(_FINAL)
	friend debug::DebugHandle;
	friend class DebugRenderer;
#endif
public:
	Renderer(Synchronizer* synchronizer);
	~Renderer();

	void InitiateDebug();


	void Render();
	void DrawIBL();
	graphics::IRenderNode* GetNode(uint64 type);


	TerrainSystem* GetTerrainSystem() const { return m_TerrainSystem; }

	int RegisterLight();
	void SetDirection(const CU::Vector3f& dir) { m_Direction = dir; }
	CU::Vector3f GetLightDirection() { return m_Direction; }
	Camera* GetDirectionalCamera() { return m_DirectionalShadow.GetCamera(); }

	void MakeCubemap(CU::Vector3f positon, int32 max_resolution, int32 min_resolution = 16);

	graphics::RenderContext& GetRenderContext() { return m_RenderContext; }

	float font_scale = 1.f;
private:
	float m_CubemapTimer = 0.f;


	void PrepareFrame();

	CU::GrowingArray<graphics::IRenderNode*> m_RenderNodes;
	CU::GrowingArray<Terrain*> myTerrainArray;

	CU::Vector3f		m_Direction;
	CU::Vector3f		m_OriginalDirection;
	CU::Matrix33f		m_Orientation;

	graphics::GBuffer m_GBuffer;
	graphics::RenderContext		m_RenderContext;

	graphics_deprecated::LightPass*		m_LightPass = nullptr;
	ShadowPass					m_ShadowPass;

	PostProcessManager			m_PostProcessManager;


	Quad* m_Background = nullptr;
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
	CText*						m_Text				= nullptr;
	Texture*					m_DepthTexture			= nullptr;

	Line3D*					m_Line					= nullptr;

	CEmitterInstance*			m_ParticleEmitter		= nullptr;

	//How do we handle this?
	ShadowDirectional			m_DirectionalShadow;

	//this should be an object in the world and the render part should be separated
	class WaterPlane*			m_WaterPlane				= nullptr; //Shouldn't be in here

	
	graphics::ConstantBuffer m_PixelBuffer;
	graphics::ConstantBuffer m_ViewProjection;


	TerrainSystem* m_TerrainSystem = nullptr;

#if !defined(_PROFILE) && !defined(_FINAL)
	bool m_RenderLines = false;
	bool m_LightModelWireframe = false;
	bool terrainWireframe = false;



	Texture* m_DebugTexture = nullptr;
	Quad* m_DebugQuad = nullptr;
	void WriteDebugTextures();

	bool m_CreateCubemaps = false;
	Texture* m_Cubemap = nullptr;
public:
	void SetRenderLines(bool render_lines) { m_RenderLines = render_lines; }
	bool GetRenderLines() { return m_RenderLines; }
	DeferredRenderer* GetDeferredRenderer() {	return m_DeferredRenderer; };
	void CreateCubemaps() { m_CreateCubemaps = true; }

#endif


};
