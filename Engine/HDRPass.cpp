#include "stdafx.h"
#include "HDRPass.h"
#include <Utilities.h>
#include "Window.h"
#include "Engine.h"
#include "DirectX11.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include <Engine/profile_defines.h>

#include <Engine/Viewport.h>
#include <Engine/Quad.h>

#include <input/InputHandle.h>
#include <input/InputWrapper.h>

void HDRPass::Initiate()
{



	WindowSize window_size = Engine::GetInstance()->GetInnerSize();

	int32 pow2 = cl::nearest_Pow_Under(window_size.m_Height);

	m_DefaultViewport = Engine::GetAPI()->CreateViewport(pow2, pow2, 0.f, 1.f, 0, 0);

	TextureDesc desc;
	
	desc.m_ResourceTypeBinding = graphics::BIND_RENDER_TARGET | graphics::BIND_SHADER_RESOURCE;
	desc.m_Width = window_size.m_Width;
	desc.m_Height = window_size.m_Height;
	desc.m_TextureFormat = graphics::RGBA8_UNORM;
	desc.m_ShaderResourceFormat = graphics::RGBA8_UNORM;
	desc.m_RenderTargetFormat = graphics::RGBA8_UNORM;

	m_HDRTexture = new Texture;
	m_HDRTexture->Initiate(desc,  "HDRPass | HDRTexture");

	int32 downsample_amount = int32(log(__min(window_size.m_Width, window_size.m_Height)) / log(2.f)) + 1; //can be changed

	desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;

	m_Downsampler.Initiate(downsample_amount, pow2, pow2, desc, true);

	m_HDREffect = Engine::GetInstance()->GetEffect("Shaders/tonemapping.json");
	m_RenderToScreenEffect = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");
	std::vector<cl::File> filesInFolder = cl::FindFilesInDirectory("Data/Textures/lut/*.*");
	for (auto file : filesInFolder)
	{
		Texture* t = new Texture;
		if(!m_ColorGradingTex)
			m_ColorGradingTex = t;

		std::string path("Data/Textures/lut/");
		path += file.filename;

		t->Create3DTexture(path.c_str(), 16, 16, 0, file.filename);
#ifdef _DEBUG
		debug::DebugHandle::GetInstance()->AddLUT(file.filename, t);
#endif
	}





	m_Quad = new Quad;
}

void HDRPass::CleanUp()
{
	SAFE_DELETE(m_DefaultViewport);
	SAFE_DELETE(m_HDRTexture);
	SAFE_DELETE(m_Quad);
	//m_Downsamples.DeleteAll();
}

void HDRPass::Process(Texture* scene_texture, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	//set buffers


	graphics::IGraphicsContext& ctx = render_context.GetContext();
	ctx.SetViewport(m_DefaultViewport);


	m_Downsampler.Process(scene_texture, render_context);

	IShaderResourceView* sources[] = {
		scene_texture->GetShaderView(),
		m_Downsampler.GetSample(m_Downsampler.SampleCount() - 1)->GetShaderView(),
		m_Downsampler.GetSample(0)->GetShaderView(),
	};

	Tonemapping(m_HDRTexture->GetRenderTargetView(), sources, ARRSIZE(sources));

	render_context.GetContext().PSSetShaderResource(0, 1, nullptr);
	render_context.GetContext().VSSetShaderResource(0, 1, nullptr);

	ctx.OMSetRenderTargets(1, scene_texture->GetRenderTargetRef(), nullptr);
	render_context.GetAPI().SetDefaultTargets();
	m_RenderToScreenEffect->AddShaderResource(m_HDRTexture, TextureSlot::DIFFUSE);
	m_Quad->Render(false, m_RenderToScreenEffect);

}

void HDRPass::Tonemapping(IRenderTargetView* target, IShaderResourceView* source[], int32 resource_count)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.ClearRenderTarget(target, clearcolor::black);
	ctx.OMSetRenderTargets(1, &target, nullptr);
	

	ctx.PSSetSamplerState(0, 1, graphics::MSAA_x16);
	ctx.PSSetSamplerState(1, 1, graphics::LINEAR_WRAP);

	m_HDREffect->AddShaderResource(source[0], TextureSlot::REGISTER_0);
	m_HDREffect->AddShaderResource(source[1], TextureSlot::REGISTER_1);
	m_HDREffect->AddShaderResource(source[2], TextureSlot::REGISTER_2);
	m_HDREffect->AddShaderResource(m_ColorGradingTex, TextureSlot::REGISTER_3);
	m_Quad->Render(false, m_HDREffect);
}

void HDRPass::OnResize()
{
}