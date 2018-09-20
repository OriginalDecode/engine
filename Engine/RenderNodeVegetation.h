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

	private:

		IVertexShader* m_VertexShader;
		IFragmentShader* m_PixelShader;

		Model* m_Model = nullptr;



	};

};