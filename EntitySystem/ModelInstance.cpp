#include "ModelInstance.h"
#include <Engine/BaseModel.h>

void ModelInstance::SetModel(BaseModel* const model)
{
	m_Model = model;
}

void ModelInstance::SetOrientation(const CU::Matrix44f orientation)
{
	m_Orientation = orientation;
}

void ModelInstance::Draw(const graphics::RenderContext& rc) const
{
	m_Model->Render(rc);
}
