#pragma once
#include "RenderNode.h"

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
		void AddInstance(const ModelInstance& instance) override;


	private:
		CompiledShader* m_Shaders[eShaderType::NOF_TYPES];

		std::vector<ModelInstance> m_Models;


	};
};