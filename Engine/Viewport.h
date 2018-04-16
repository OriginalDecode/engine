#pragma once
#include <Engine/engine_shared.h>
namespace graphics
{
	class Viewport
	{
	public:
		Viewport() = default;
		Viewport(const Viewport& viewport);
		Viewport(s32 width, s32 height, s32 top_left_x, s32 top_left_y, float max_depth, float min_depth, IViewport* viewport);
		~Viewport();

		IViewport* GetViewport() { return m_Viewport; }

		void OnResize();
		void OnAltEnter();
		
		static void Copy(Viewport* dest, Viewport* src);

		s32 GetWidth() const { return m_Width; }
		s32 GetHeight() const { return m_Height; }
		s32 GetTopLeftX() const { return m_TopLeftX; }
		s32 GetTopLeftY() const { return m_TopLeftY; }
		float GetMaxDepth() const { return m_MaxDepth; }
		float GetMinDepth() const { return m_MinDepth; }

		void SetWidth(s32 width) { m_Width = width; }
		void SetHeight(s32 height) { m_Height = height; }
		void SetTopLeftX(s32 top_left_x) { m_TopLeftX = top_left_x; }
		void SetTopLeftY(s32 top_left_y) { m_TopLeftY = top_left_y; }
		void SetMaxDepth(float max_depth) { m_MaxDepth = max_depth; }
		void SetMinDepth(float min_depth) { m_MinDepth = min_depth; }

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