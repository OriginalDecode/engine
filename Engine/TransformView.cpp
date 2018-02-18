#include "stdafx.h"
#include "TransformView.h"

#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>

#include <EntitySystem/TranslationComponent.h>
#include <EntitySystem/PhysicsComponent.h>
#include <EntitySystem/GraphicsComponent.h>

#include <Physics/RigidBody.h>



TransformView::TransformView(EntityManager& em)
	: InspectorView(em)
{
}


TransformView::~TransformView()
{
}

void TransformView::Update()
{
	if (m_CurrentEntity <= 0 || !(m_ComponentFlags & TreeDweller::TRANSLATION))
		return;
	
// 	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
// 	{
// 		Camera* camera = Engine::GetInstance()->GetCamera();
// 		const CU::Matrix44f orientation = CU::Math::Inverse(camera->GetOrientation());
// 		CU::Matrix44f perspective = camera->GetPerspective();
// 
// 		TranslationComponent& translation = m_Manager.GetComponent<TranslationComponent>(m_CurrentEntity);
// 		EditTransform(orientation.myMatrix, perspective.myMatrix, translation.m_Orientation.myMatrix);
// 
// 		if (m_ComponentFlags & TreeDweller::PHYSICS)
// 		{
// 			PhysicsComponent& physics = m_Manager.GetComponent<PhysicsComponent>(m_CurrentEntity);
// 
// // 			if (ImGuizmo::IsUsing())
// // 			{
// // 				physics.m_Body->SetPosition(translation.m_Orientation.GetPosition());
// // 
// // 				GraphicsComponent& g = m_Manager.GetComponent<GraphicsComponent>(m_CurrentEntity);
// // 				physics.m_Body->SetScale(g.m_Instances[0].m_Scale);
// // 			}
// 		}
// 
// 
// 	}
}
//
//void TransformView::EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
//{
//	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
//	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
//	static bool useSnap = false;
//	static float snap[3] = { 1.f, 1.f, 1.f };
//
//	constexpr int w_key = 'W';
//	constexpr int r_key = 'R';
//	constexpr int e_key = 'E';
//	constexpr int s_key = 'S';
//
//
//	if (ImGui::IsKeyPressed(w_key))
//		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
//	if (ImGui::IsKeyPressed(e_key))
//		mCurrentGizmoOperation = ImGuizmo::ROTATE;
//
//	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
//		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
//	ImGui::SameLine();
//	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
//		mCurrentGizmoOperation = ImGuizmo::ROTATE;
//	
//	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
//	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
//	ImGui::InputFloat3("Tr", matrixTranslation, 3);
//	ImGui::InputFloat3("Rt", matrixRotation, 3);
//	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
//	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
//	{
//		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
//			mCurrentGizmoMode = ImGuizmo::LOCAL;
//		ImGui::SameLine();
//		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
//			mCurrentGizmoMode = ImGuizmo::WORLD;
//	}
//
//
//	if (ImGui::IsKeyPressed(s_key))
//		useSnap = !useSnap;
//	ImGui::Checkbox("", &useSnap);
//	ImGui::SameLine();
//
//	switch (mCurrentGizmoOperation)
//	{
//	case ImGuizmo::TRANSLATE:
//		ImGui::InputFloat3("Snap", &snap[0]);
//		break;
//	case ImGuizmo::ROTATE:
//		ImGui::InputFloat("Angle Snap", &snap[0]);
//		break;
//	}
//	ImGuiIO& io = ImGui::GetIO();
//	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
//	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
//}