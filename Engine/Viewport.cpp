#include "stdafx.h"
#include "Viewport.h"


namespace graphics
{
	Viewport::Viewport(s32 width, s32 height, s32 top_left_x, s32 top_left_y, float max_depth, float min_depth, IViewport* viewport)
		: m_Width(width)
		, m_Height(height)
		, m_TopLeftX(top_left_x)
		, m_TopLeftY(top_left_y)
		, m_MaxDepth(max_depth)
		, m_MinDepth(min_depth)
		, m_Viewport(viewport)
	{
	}


	Viewport::~Viewport()
	{
		delete m_Viewport;
		m_Viewport = nullptr;
	}

	void Viewport::OnResize()
	{

	}

	void Viewport::OnAltEnter()
	{

	}

	void Viewport::Copy(Viewport* dest, Viewport* src)
	{
		dest->m_Height = src->m_Height;
		dest->m_Width = src->m_Width;
		dest->m_TopLeftX = src->m_TopLeftX;
		dest->m_TopLeftY = src->m_TopLeftY;
		dest->m_MaxDepth = src->m_MaxDepth;
		dest->m_MinDepth = src->m_MinDepth;
	}

};