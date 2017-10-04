#pragma once
#include <Engine/engine_shared.h>
#include <Engine/Model.h>

class LightModel : public Model
{
	friend class CModelImporter;
public:
	LightModel();
	~LightModel();
	void Initiate(const std::string& filename) override;
	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;
	void AddChild(LightModel* child);
	void SetOrientation(const CU::Matrix44f& orientation);
private:

	CU::GrowingArray<LightModel*> m_Children;
			
};

