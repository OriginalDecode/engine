#pragma once
#include <Engine/engine_shared.h>
namespace graphics
{
	class Viewport
	{
	public:
		Viewport() = default;
		Viewport(const Viewport& viewport);
		Viewport(int32 width, int32 height, int32 top_left_x, int32 top_left_y, float max_depth, float min_depth, IViewport* viewport);
		~Viewport();

		IViewport* GetViewport() { return m_Viewport; }

		void OnResize();
		void OnAltEnter();
		
		static void Copy(Viewport* dest, Viewport* src);

		int32 GetWidth() const { return m_Width; }
		int32 GetHeight() const { return m_Height; }
		int32 GetTopLeftX() const { return m_TopLeftX; }
		int32 GetTopLeftY() const { return m_TopLeftY; }
		float GetMaxDepth() const { return m_MaxDepth; }
		float GetMinDepth() const { return m_MinDepth; }

		void SetWidth(int32 width) { m_Width = width; }
		void SetHeight(int32 height) { m_Height = height; }
		void SetTopLeftX(int32 top_left_x) { m_TopLeftX = top_left_x; }
		void SetTopLeftY(int32 top_left_y) { m_TopLeftY = top_left_y; }
		void SetMaxDepth(float max_depth) { m_MaxDepth = max_depth; }
		void SetMinDepth(float min_depth) { m_MinDepth = min_depth; }

	private:
		IViewport* m_Viewport = nullptr;
		int32 m_Width = 0;
		int32 m_Height = 0;
		
		int32 m_TopLeftX = 0;
		int32 m_TopLeftY = 0;

		float m_MaxDepth = 0;
		float m_MinDepth = 0;


	};
};