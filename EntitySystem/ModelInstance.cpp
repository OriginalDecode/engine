#include "ModelInstance.h"
#include <Engine/BaseModel.h>
#include <Engine/Material.h>

ModelInstance::~ModelInstance()
{
}

void ModelInstance::SetModel(BaseModel* const model)
{
	m_Model = model;
}

void ModelInstance::SetOrientation(const CU::Matrix44f orientation)
{
	m_Orientation = orientation;
}

void ModelInstance::SetMaterial(Material* const pMaterial)
{
	m_Material = pMaterial;
	m_Key = m_Material->GetKey();
}

void ModelInstance::Draw(const graphics::RenderContext& rc) const
{
	
	//m_Model->SetOrientation(m_Orientation);
	m_Model->Render(rc);
}

void ModelInstance::SetSurface(Surface* const pSurface)
{
	m_Surface = pSurface;
}

void ModelInstance::UpdateMaterial()
{
	if (m_Surface && m_Material)
	{
		m_Model->SetSurface0(m_Surface);
	}
}

uint64 ModelInstance::GetMaterialKey() const
{
	return m_Key;
}

void ModelInstance::SetMaterialKey(uint64 key)
{
	m_Key = key;
}
