#pragma once
#include "RenderNode.h"


namespace graphics
{
	class RenderNodeShadows : public IRenderNode
	{
	public:
		DECLARE_RENDER_NODE(RenderNodeShadows);

		RenderNodeShadows();
		~RenderNodeShadows();



		void Draw(const RenderContext& rc) override;
		void Reload(CompiledShader* shader) override;
		void AddInstance(const ModelInstance instance) override;


	private:

		std::vector<ModelInstance> m_Models;


	};
};