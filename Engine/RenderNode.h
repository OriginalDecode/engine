#pragma once


#include <engine/ShaderFactory.h>
namespace graphics
{
	class RenderContext;
	class IRenderNode : public ShaderReload
	{
	public:
		IRenderNode() = default;
		virtual ~IRenderNode() { };

		virtual void Draw(const RenderContext& rc) = 0;

		virtual void Reload(CompiledShader* shader) = 0;

	private:

	};


};