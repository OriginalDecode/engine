#pragma once


#include <engine/ShaderFactory.h>
#include "ModelInstance.h"

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

		virtual void AddInstance(const ModelInstance& instance) = 0;

		u64 GetType() const { return m_Type;  }
	protected:
		u64 m_Type;
	};


};