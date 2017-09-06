#pragma once
#include <Engine/engine_shared.h>
#include <Engine/Model.h>

class LightModel : public Model
{
	friend class CModelImporter;
public:
	LightModel() = default;
	~LightModel();
	void Initiate(const std::string& filename) override;
	void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;
	void AddChild(LightModel* aChild);
	void SetOrientation(const CU::Matrix44f& orientation);
private:

	CU::GrowingArray<LightModel*> myChildren;
			
};

