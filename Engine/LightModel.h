#pragma once
#include <Engine/snowblind_shared.h>
#include <Engine/Model.h>

class LightModel : public Model
{
public:
	LightModel() = default;
	bool CleanUp() override;
	void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context) override;


};

