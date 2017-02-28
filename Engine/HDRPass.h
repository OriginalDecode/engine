#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include "Window.h"
namespace Hex
{
	class Texture;
	class Effect;
	class Engine;
	class DirectX11;
}

class HDRPass
{
public:
	HDRPass() = default;
	void Initiate();
	void CleanUp();
	void Process(Hex::Texture* scene_texture);
	void OnResize();
	bool toggle_debug = false;
private:
	void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);
	void Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count);

	void FillViewportData(Viewport& viewport);

	float y_height = 0.f;
	float x_width = 0.f;

	CU::GrowingArray<Hex::Texture*> m_Downsamples;

	Hex::Texture* m_Clearcolor = nullptr;

	Hex::SWindowSize	m_WindowSize;
	Hex::Effect*		m_HDREffect				= nullptr;
	Hex::Texture*		m_HDRTexture			= nullptr;
	Hex::Effect*		m_DownsampleEffect		= nullptr;
	Hex::Effect*		m_RenderToScreenEffect	= nullptr;
	Hex::Engine*		m_Engine				= nullptr;
	Hex::DirectX11*		m_API					= nullptr;
};

