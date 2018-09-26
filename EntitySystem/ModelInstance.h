#pragma once
#include <Math/Matrix/Matrix44.h>

class BaseModel;
namespace graphics
{
	class RenderContext;
};

class ModelInstance
{
public:
	ModelInstance() = default;

	void SetModel(BaseModel* const model);
	void SetOrientation(const CU::Matrix44f orientation);
	void Draw(const graphics::RenderContext& rc) const;


private:
	BaseModel* m_Model = nullptr;
	CU::Matrix44f m_Orientation;

};