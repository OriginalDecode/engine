#pragma once
#include <Engine/snowblind_shared.h>
#include <Engine/Model.h>
#include <DataStructures/GrowingArray.h>

class AtmosphereModel : public Model
{
	friend class CModelImporter;
public:
	AtmosphereModel() = default;
	void Initiate(const std::string& filename) override;
	void CleanUp();


	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;
	void AddChild(AtmosphereModel* child);
	void SetOrientation(const CU::Matrix44f& orientation);
private:

	CU::GrowingArray<AtmosphereModel*> myChildren;

};

