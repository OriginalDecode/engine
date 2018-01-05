#include "stdafx.h"
#include "GraphicsView.h"


#include <Engine/imgui.h>

GraphicsView::GraphicsView(EntityManager& em)
	: TransformView(em)
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

		Camera* camera = Engine::GetInstance()->GetCamera();
		const CU::Matrix44f orientation = CU::Math::Inverse(camera->GetOrientation());
		CU::Matrix44f perspective = camera->GetPerspective();

		//need a list of instances on this entity.

		//TranslationComponent& translation = m_Manager.GetComponent<TranslationComponent>(m_CurrentEntity);
		EditTransform(orientation.myMatrix, perspective.myMatrix, NULL);

	}
}
