#pragma once
#include "RenderNode.h"

class ShadowDirectional;
namespace graphics
{
	class RenderNodeShadows : public IRenderNode
	{
	public:
		DECLARE_RENDER_NODE(RenderNodeShadows);

		RenderNodeShadows();
		~RenderNodeShadows();

		void Init(ShadowDirectional* shadowDirectional);

		void Draw(const RenderContext& rc) override;
		void Reload(CompiledShader* shader) override;
		void AddInstance(const ModelInstance instance) override;


	private:
		CompiledShader* m_Shaders[2];
		bool m_Processed = false;
		std::unordered_map<uint64, std::vector<ModelInstance>> m_Models;
		ShadowDirectional* m_DirectionalShadow = nullptr;


	};
};