#pragma once

namespace Snowblind
{
	class CFullscreenQuad
	{
	public:
		CFullscreenQuad();
		virtual ~CFullscreenQuad();

		void InitFullscreenQuad(CEffect* anEffect);
		void ActiveFullscreenQuad();
		void RenderFullscreenQuad(CEffect* anEffect, const std::string& aTechnique);
	protected:

	};
};