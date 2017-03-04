#include "stdafx.h"
#include "MoveArrowModel.h"
#include "Texture.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Surface.h"
#include "Model.h"

MoveArrowModel::MoveArrowModel()
{
}

void MoveArrowModel::Initiate()
{



	m_Forward.m_Key = Engine::GetInstance()->LoadModel("Data/Model/blue_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Forward.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/blue.dds");

	m_Forward.m_Arrow = Engine::GetInstance()->GetModel(m_Forward.m_Key);
	CU::GrowingArray<CModel*> children = m_Forward.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Forward.m_DirColor->GetShaderView());
		}

	}


	m_Forward.m_Orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(90.f) * -1) * m_Forward.m_Orientation;


	m_Up.m_Key = Engine::GetInstance()->LoadModel("Data/Model/green_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Up.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/green.dds");

	m_Up.m_Arrow = Engine::GetInstance()->GetModel(m_Up.m_Key);

	children = m_Up.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Up.m_DirColor->GetShaderView());
		}

	}
	m_Up.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * 1) * m_Up.m_Orientation;

	m_Right.m_Key = Engine::GetInstance()->LoadModel("Data/Model/red_arrow.fbx", "Data/Shaders/T_Movement_Arrow.json");

	m_Right.m_DirColor = Engine::GetInstance()->GetTexture("Data/Textures/red.dds");

	m_Right.m_Arrow = Engine::GetInstance()->GetModel(m_Right.m_Key);
	children = m_Right.m_Arrow->GetChildModels();
	for (CModel* child : children)
	{
		CU::GrowingArray<CSurface*>& surfaces = child->GetSurfaces();
		for (CSurface* surface : surfaces)
		{
			surface->ClearTextures();
			surface->AddTexture(m_Right.m_DirColor->GetShaderView());
		}

	}
	//m_Right.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * 1) * m_Right.m_Orientation;

	m_Up.Initiate();
	m_Right.Initiate();
	m_Forward.Initiate();

	//__________________________
	m_Up.m_MaxPos = m_Up.m_Arrow->GetWHD();
	m_Up.m_MinPos = { -m_Up.m_MaxPos.x, -m_Up.m_MaxPos.y, -m_Up.m_MaxPos.z };

	//__________________________
	m_Right.m_MaxPos = m_Right.m_Arrow->GetWHD();
	m_Right.m_MinPos = { -m_Right.m_MaxPos.x, -m_Right.m_MaxPos.y, -m_Right.m_MaxPos.z };

	//__________________________
	m_Forward.m_MaxPos = m_Forward.m_Arrow->GetWHD();
	m_Forward.m_MinPos = { -m_Forward.m_MaxPos.x, -m_Forward.m_MaxPos.y, -m_Forward.m_MaxPos.z };



	m_Forward.direction = DirectionalArrow::eDirection::FORWARD;
	m_Right.direction = DirectionalArrow::eDirection::RIGHT;
	m_Up.direction = DirectionalArrow::eDirection::UP;


}

void MoveArrowModel::CleanUp()
{

}

void MoveArrowModel::Render()
{
	
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	CU::Vector4f scale = CU::Vector4f(distance, distance, distance, 1);
	//CU::Matrix44f scale_matrix = CU::Matrix44f::CreateScaleMatrix(scale);
	CU::Matrix44f t = m_Right.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Right.m_Key, t));

	//__________________________
	
	t = m_Forward.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Forward.m_Key, t));

	//__________________________

	t = m_Up.m_Orientation;
	//t = CU::Matrix44f::CreateScaleMatrix(scale) * t;
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::MODEL_NO_DEFERRED, m_Up.m_Key, t));


	
	
	//Update();
}

void MoveArrowModel::Update()
{
	m_Forward.Update();
	m_Up.Update();
	m_Right.Update();

	/**m_Forward.m_Orientation.SetPosition(m_Orientation.GetPosition());
	m_Up.m_Orientation.SetPosition(m_Orientation.GetPosition());
	m_Right.m_Orientation.SetPosition(m_Orientation.GetPosition());/**/
}

void MoveArrowModel::SetPosition(const CU::Vector3f& pos)
{
	m_Orientation.SetPosition(pos);

	m_Forward.m_Orientation.SetPosition(pos);

	m_Up.m_Orientation.SetPosition(pos);

	m_Right.m_Orientation.SetPosition(pos);

}

void MoveArrowModel::RenderBoxes()
{
	m_Forward.RenderBox();
	m_Up.RenderBox();
	m_Right.RenderBox();
}

void DirectionalArrow::Initiate()
{
	CU::Plane<float> plane0;

	CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f right = m_Orientation.GetRight();
	CU::Vector4f forward = m_Orientation.GetForward();


	CU::Vector4f position;
	CU::Vector3f whd = m_Arrow->GetWHD();
	//x

	position = m_Orientation.GetTranslation();
	position += right * whd.x;
	plane0.InitWithPointAndNormal(position, right);
	m_OBB.AddPlane(plane0);

	position = m_Orientation.GetTranslation();
	position -= right * whd.x;
	right -= (right * 2.f);
	plane0.InitWithPointAndNormal(position, right);
	m_OBB.AddPlane(plane0);

	//y
	position = m_Orientation.GetTranslation();
	position += up * whd.y;
	plane0.InitWithPointAndNormal(position, up);
	m_OBB.AddPlane(plane0);

	position = m_Orientation.GetTranslation();
	position -= up * whd.y;
	up -= (up * 2.f);
	plane0.InitWithPointAndNormal(position, up);
	m_OBB.AddPlane(plane0);

	//z
	position = m_Orientation.GetTranslation();
	position += forward * whd.z;
	plane0.InitWithPointAndNormal(position, forward);
	m_OBB.AddPlane(plane0);


	position = m_Orientation.GetTranslation();
	position -= forward * whd.z;
	forward -= (forward * 2.f);
	plane0.InitWithPointAndNormal(position, forward);
	m_OBB.AddPlane(plane0);
}

DirectionalArrow::~DirectionalArrow()
{
}

bool DirectionalArrow::Inside(const CU::Vector3f& position)
{
	/*
		CU::Vector3f max = m_Orientation.GetPosition() + m_MaxPos;
		CU::Vector3f min = m_Orientation.GetPosition() + m_MinPos;


		if (position < max && position > min)
			return true;*/
	return m_OBB.Inside(position);
	//return false;
}

void DirectionalArrow::RenderBox()
{
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	SLinePoint p1, p2, p3, p4, p5, p6, p7, p8;
	p1.color = CU::Vector4f(1, 1, 1, 1);
	p2.color = p1.color;
	p3.color = p1.color;
	p4.color = p1.color;
	p5.color = p1.color;
	p6.color = p1.color;
	p7.color = p1.color;
	p8.color = p1.color;

	p1.position = m_Orientation.GetTranslation(); // translation.myOrientation.GetTranslation();
	p2.position = p1.position;
	p3.position = p1.position;
	p4.position = p1.position;
	p5.position = p1.position;
	p6.position = p1.position;
	p7.position = p1.position;
	p8.position = p1.position;
	CU::Vector3f whd = m_Arrow->GetWHD() /** distance*/;
	p1.position -= m_Orientation.GetRight() * whd.x;
	p1.position -= m_Orientation.GetUp() * whd.y;
	p1.position -= m_Orientation.GetForward() * whd.z;

	p2.position += m_Orientation.GetRight() * whd.x;
	p2.position -= m_Orientation.GetUp() * whd.y;
	p2.position -= m_Orientation.GetForward() * whd.z;

	p3.position += m_Orientation.GetRight() * whd.x;
	p3.position -= m_Orientation.GetUp() * whd.y;
	p3.position += m_Orientation.GetForward() * whd.z;

	p4.position -= m_Orientation.GetRight() * whd.x;
	p4.position -= m_Orientation.GetUp() * whd.y;
	p4.position += m_Orientation.GetForward() * whd.z;

	p5.position -= m_Orientation.GetRight() * whd.x;
	p5.position += m_Orientation.GetUp() * whd.y;
	p5.position -= m_Orientation.GetForward() * whd.z;

	p6.position -= m_Orientation.GetRight() * whd.x;
	p6.position += m_Orientation.GetUp() * whd.y;
	p6.position += m_Orientation.GetForward() * whd.z;

	p7.position += m_Orientation.GetRight() * whd.x;
	p7.position += m_Orientation.GetUp() * whd.y;
	p7.position -= m_Orientation.GetForward() * whd.z;

	p8.position += m_Orientation.GetRight() * whd.x;
	p8.position += m_Orientation.GetUp() * whd.y;
	p8.position += m_Orientation.GetForward() * whd.z;

	Engine* engine = Engine::GetInstance();
	Synchronizer* synch = engine->GetSynchronizer();
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p1, p2));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p2, p3));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p3, p4));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p4, p1));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p1, p5));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p5, p6));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p6, p8));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p8, p7));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p7, p5));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p6, p4));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p7, p2));
	synch->AddRenderCommand(RenderCommand(eType::LINE_Z_DISABLE, p8, p3));
}

void DirectionalArrow::Update()
{
	CU::Vector4f camera_position = Engine::GetInstance()->GetCamera()->GetPosition();
	float distance = CU::Math::Length(m_Orientation.GetTranslation() - camera_position) * 0.5f;
	CU::Vector4f up = m_Orientation.GetUp();
	CU::Vector4f right = m_Orientation.GetRight();
	CU::Vector4f forward = m_Orientation.GetForward();


	CU::Vector4f position;
	CU::Vector3f whd = m_Arrow->GetWHD() /** distance*/;
	//x

	position = m_Orientation.GetTranslation();
	position += right * whd.x;
	m_OBB.m_Planes[0].InitWithPointAndNormal(position, right);

	position = m_Orientation.GetTranslation();
	position -= right * whd.x;
	right -= (right * 2.f);
	m_OBB.m_Planes[1].InitWithPointAndNormal(position, right);

	//y
	position = m_Orientation.GetTranslation();
	position += up * whd.y;
	m_OBB.m_Planes[2].InitWithPointAndNormal(position, up);

	position = m_Orientation.GetTranslation();
	position -= up * whd.y;
	up -= (up * 2.f);
	m_OBB.m_Planes[3].InitWithPointAndNormal(position, up);

	//z
	position = m_Orientation.GetTranslation();
	position += forward * whd.z;
	m_OBB.m_Planes[4].InitWithPointAndNormal(position, forward);


	position = m_Orientation.GetTranslation();
	position -= forward * whd.z;
	forward -= (forward * 2.f);
	m_OBB.m_Planes[5].InitWithPointAndNormal(position, forward);
}
