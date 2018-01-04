#include "stdafx.h"
#include "GraphicsView.h"


#include <Engine/imgui.h>

GraphicsView::GraphicsView(EntityManager& em)
	: InspectorView(em)
{
}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::Update()
{
	if (!(m_ComponentFlags & TreeDweller::GRAPHICS))
		return;

	ImGui::Text("Hello world");

	if (ImGui::CollapsingHeader("Graphics"))
	{

	}
}
