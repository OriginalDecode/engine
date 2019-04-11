#pragma once
#include "RenderNode.h"

class ShadowDirectional;
namespace graphics
{
	class RenderNodeShadows : public IRenderNode
	{
	public:
		DECLARE_RENDER_NODE(RenderNodeShadows);

		RenderNodeShadows(ShadowDirectional* shadowDirectional);

		void Draw(const RenderContext& rc) override;
		void Reload(CompiledShader* shader) override;
		void AddInstance(const ModelInstance instance) override;

		void Reset();

	private:
		CompiledShader* m_Shaders[2];
		bool m_Processed = false;
		std::unordered_map<uint64, std::vector<ModelInstance>> m_Models;
		ShadowDirectional* m_DirectionalShadow = nullptr;


	};
};