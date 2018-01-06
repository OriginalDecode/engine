#include "stdafx.h"
#include "GraphicsView.h"

#include <Engine/imgui.h>
#include <Engine/ImGuizmo.h>

#include <EntitySystem/GraphicsComponent.h>
#include <EntitySystem/TranslationComponent.h>


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

		Camera* camera = Engine::GetInstance()->GetCamera();
		const CU::Matrix44f orientation = CU::Math::Inverse(camera->GetOrientation());
		CU::Matrix44f perspective = camera->GetPerspective();

		//need a list of instances on this entity.

		GraphicsComponent& c = m_Manager.GetComponent<GraphicsComponent>(m_CurrentEntity);
		TranslationComponent& t = m_Manager.GetComponent<TranslationComponent>(m_CurrentEntity);

		const CU::Matrix44f original_mat = t.m_Orientation;
		CU::Matrix44f entity_mat = c.m_Instances[0].m_Orientation * t.m_Orientation;

		EditTransform(orientation.myMatrix, perspective.myMatrix, entity_mat.myMatrix);

		entity_mat.SetPosition(entity_mat.GetPosition() - original_mat.GetPosition());

		c.m_Instances[0].m_Orientation = entity_mat;



	}
}



void GraphicsView::EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };

	constexpr int w_key = 'W';
	constexpr int r_key = 'R';
	constexpr int e_key = 'E';
	constexpr int s_key = 'S';


	if (ImGui::IsKeyPressed(w_key))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(e_key))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(r_key)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	if (mCurrentGizmoOperation == ImGuizmo::SCALE)
		mCurrentGizmoMode = ImGuizmo::LOCAL;

	if (ImGui::IsKeyPressed(s_key))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();

	switch (mCurrentGizmoOperation)
	{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
	}
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
}