#include "stdafx.h"
#include "BlurPass.h"

#include "Quad.h"
#include "Effect.h"
#include "Engine.h"


BlurPass::BlurPass()
{
	m_Quad = new Quad();
}


BlurPass::~BlurPass()
{
}

//we can have multiple blur passes with different effects
void BlurPass::Initiate(const char* effect)
{

}

void BlurPass::Process(Texture* scene, const graphics::RenderContext& rc)
{

}

void BlurPass::OnResize()
{

}
