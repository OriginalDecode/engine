#pragma once
#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include "Window.h"

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
private:
	void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);
	void Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count);

	CU::GrowingArray<Texture*> m_Downsamples;

	graphics::Viewport* m_DefaultViewport = nullptr;
	graphics::Viewport* m_ChangeableViewport = nullptr;

	Texture*	m_HDRTexture = nullptr;
	Quad*		m_Quad = nullptr;
	Effect*		m_HDREffect = nullptr;
	Effect*		m_DownsampleEffect = nullptr;
	Effect*		m_RenderToScreenEffect = nullptr;
};

