#include "stdafx.h"
#include "ShadowPass.h"
namespace Snowblind
{

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

};