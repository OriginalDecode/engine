#pragma once
#include "RenderNode.h"
#include <unordered_map>

struct CompiledShader;
namespace graphics
{
	class RenderNodeGeneral : public IRenderNode
	{
	public:
		RenderNodeGeneral();
		~RenderNodeGeneral();

		void Draw(const RenderContext& rc) override;
		void Reload(CompiledShader* shader) override;
		void AddInstance(const ModelInstance instance) override;

		static uint64 Type;
		uint64 GetType() const { return Type; }
	private:
		CompiledShader* m_Shaders[eShaderType::NOF_TYPES];
		CompiledShader* m_DepthShader = nullptr;

		bool m_DrawDepth = true;

		std::unordered_map<uint64, std::vector<ModelInstance>> m_Models;


	};
};