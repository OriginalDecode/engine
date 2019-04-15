#pragma once
#include "RenderNode.h"
#include <unordered_map>

struct CompiledShader;
namespace graphics
{
	class RenderNodeGeneral : public IRenderNode
	{
	public:
		DECLARE_RENDER_NODE(RenderNodeGeneral);
		RenderNodeGeneral();
		~RenderNodeGeneral() = default;

		void Draw(const RenderContext& rc) override;
		void Reload(CompiledShader* shader) override;
		void AddInstance(const ModelInstance instance) override;

	private:
		CompiledShader* m_VertShader = nullptr;
		CompiledShader* m_FragShader = nullptr;
		CompiledShader* m_DepthShader = nullptr;

		bool m_DrawDepth = true;

		std::unordered_map<uint64, std::vector<ModelInstance>> m_Models;


	};
};