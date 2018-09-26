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
		void AddInstance(const ModelInstance instance) override;


		static u64 Type;
		u64 GetType() const { return Type; }
	private:
		CompiledShader* m_Shaders[eShaderType::NOF_TYPES];

		std::vector<ModelInstance> m_Models;


	};
};