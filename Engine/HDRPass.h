#pragma once
#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include "Window.h"

class Texture;
class Effect;
class Engine;
class DirectX11;

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
	bool toggle_debug = false;
private:
	void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);
	void Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count);

	void FillViewportData(graphics::Viewport& viewport);

	float y_height = 0.f;
	float x_width = 0.f;

	CU::GrowingArray<Texture*> m_Downsamples;


	WindowSize	m_WindowSize;
	Effect*		m_HDREffect = nullptr;
	Texture*	m_HDRTexture = nullptr;
	Effect*		m_DownsampleEffect = nullptr;
	Effect*		m_RenderToScreenEffect = nullptr;
	Engine*		m_Engine = nullptr;
	DirectX11*	m_API = nullptr;
};

