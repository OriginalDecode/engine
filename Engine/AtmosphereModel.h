#pragma once
#include <Engine/engine_shared.h>
#include <Engine/Model.h>
#include <DataStructures/GrowingArray.h>

class AtmosphereModel : public Model
{
	friend class CModelImporter;
public:
	AtmosphereModel() = default;
	~AtmosphereModel();
	void Initiate(const std::string& filename) override;

	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;

	void AddChild(AtmosphereModel* child);
	void SetOrientation(const CU::Matrix44f& orientation);

private:
	CU::GrowingArray<AtmosphereModel*> m_Children;

};

