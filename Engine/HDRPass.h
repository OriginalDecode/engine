#pragma once
#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include "Window.h"
#include <Engine/DownsamplePass.h>

class Texture;
class Effect;
class Engine;
class Quad;
namespace graphics
{
	class Viewport;
};

class HDRPass
{
public:
	HDRPass() = default;
	void Initiate();
	void CleanUp();
	void Process(Texture* scene_texture, const graphics::RenderContext& render_context);
	void OnResize();
	void SetLUT(Texture* tex) { m_ColorGradingTex = tex; }
private:
	//void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);
	void Tonemapping(IRenderTargetView* target, IShaderResourceView* source[], int32 resource_count);


	graphics::Viewport* m_DefaultViewport = nullptr;

	Texture*	m_ColorGradingTex = nullptr;
	Texture*	m_HDRTexture = nullptr;
	Quad*		m_Quad = nullptr;

	Effect*		m_HDREffect = nullptr;
	Effect*		m_RenderToScreenEffect = nullptr;


	DownsamplePass m_Downsampler;

};

