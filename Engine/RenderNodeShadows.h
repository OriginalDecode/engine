#pragma once
#include "RenderNode.h"
#include <Engine/Shadow_Directional.h>


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
		CompiledShader* m_Shaders[2];

		std::unordered_map<uint64, std::vector<ModelInstance>> m_Models;
		ShadowDirectional m_DirectionalShadow;


	};
};