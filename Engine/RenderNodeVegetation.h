#pragma once
#include "RenderNode.h"

struct CompiledShader;
class Model;
namespace graphics
{
	class RenderNodeVegetation : public IRenderNode
	{
	public:
		RenderNodeVegetation();
		~RenderNodeVegetation() override;

		void Draw(const RenderContext& rc) override;

		void Reload(CompiledShader* shader) override;

	private:

		CompiledShader* m_Shaders[eShaderType::NOF_TYPES];

		std::vector<Model*> m_Models;



	};

};