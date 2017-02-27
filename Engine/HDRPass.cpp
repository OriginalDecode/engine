#include "stdafx.h"
#include "HDRPass.h"
#include <Utilities.h>
#include "Window.h"
void HDRPass::Initiate()
{
	const Hex::SWindowSize& window_size = Hex::Engine::GetInstance()->GetWindowSize();




}

void HDRPass::CleanUp()
{
	m_Downsamples.DeleteAll();
}

void HDRPass::Process(Hex::Texture* scene_texture)
{
	
}

void HDRPass::Downsample()
{

}

