#pragma once
#include "IRenderPass.h"
namespace graphics
{
	class AtmospherePass : public IRenderPass
	{
	public:
		AtmospherePass(eRenderPass pass, IGraphicsContext* ctx);
		~AtmospherePass();

	private:

	};
};