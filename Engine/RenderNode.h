#pragma once



namespace graphics
{
	class RenderContext;
	class IRenderNode
	{
	public:
		IRenderNode() = default;
		virtual ~IRenderNode() { };

		virtual void Draw(const RenderContext& rc) = 0;



	private:

	};


};