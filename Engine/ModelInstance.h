#pragma once
#include <CommonLib/Math/Matrix/Matrix.h>

class Model;
class ModelInstance
{
public:
	ModelInstance() = default;
	ModelInstance(Model* model);

	const CU::Matrix44f& GetOrientation() const { return m_Orientation; }
	void SetOrientation(const CU::Matrix44f& orientation) { m_Orientation = orientation; }

	Model* GetModel() { return  m_ModelRef; }

	

private:
	Model* m_ModelRef;
	CU::Matrix44f m_Orientation;
};

