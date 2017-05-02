#pragma once
#include <Engine/snowblind_shared.h>
#include <Engine/Model.h>

class AtmosphereModel : public Model
{
public:
	AtmosphereModel() = default;
	bool CleanUp() override;

	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;

private:

	CU::GrowingArray<AtmosphereModel*> myChildren;

};

