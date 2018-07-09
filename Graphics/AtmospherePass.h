#pragma once
#include "RenderPass.h"
namespace graphics
{
	class AtmospherePass : public RenderPass
	{
	public:
		AtmospherePass(eRenderPass pass, IGraphicsContext* ctx);
		~AtmospherePass();

	private:

	};
};