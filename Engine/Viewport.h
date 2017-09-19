#pragma once
#include <Engine/engine_shared.h>
namespace graphics
{
	class Viewport
	{
	public:
		Viewport(s32 width, s32 height, s32 top_left_x, s32 top_left_y, float max_depth, float min_depth, IViewport* viewport);
		~Viewport();

		IViewport* GetViewport() { return m_Viewport; }

		void OnResize();
		void OnAltEnter();

	private:
		IViewport* m_Viewport = nullptr;
		s32 m_Width = 0;
		s32 m_Height = 0;
		
		s32 m_TopLeftX = 0;
		s32 m_TopLeftY = 0;

		float m_MaxDepth = 0;
		float m_MinDepth = 0;


	};
};