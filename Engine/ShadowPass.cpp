#include "stdafx.h"
#include "ShadowPass.h"


bool ShadowPass::Initiate()
{

	return true;
}

bool ShadowPass::CleanUp()
{
	m_DepthTexture->CleanUp();
	SAFE_DELETE(m_DepthTexture);

	return true;
}

