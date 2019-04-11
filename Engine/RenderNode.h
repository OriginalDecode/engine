#pragma once


#include <engine/ShaderFactory.h>
#include "ModelInstance.h"

#define DECLARE_RENDER_NODE(_class) static uint64 GetType() { static uint64 Type = cl::Hash(#_class); return Type; } uint64 GetNodeType() const override { return GetType(); }

namespace graphics
{
	class RenderContext;

	class IRenderNode : public ShaderReload
	{
	public:


		IRenderNode() = default;
		virtual ~IRenderNode() = default;

		virtual void Draw(const RenderContext& rc) = 0;
		virtual void Reload(CompiledShader* shader) = 0;

		virtual void AddInstance(const ModelInstance instance) = 0;

		virtual uint64 GetNodeType() const = 0;
	};
		

};