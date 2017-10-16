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
	void Render(const graphics::RenderContext& rc) override;
	void AddChild(LightModel* child);
	void SetOrientation(const CU::Matrix44f& orientation);
private:

	CU::GrowingArray<LightModel*> m_Children;

	struct cbVertex
	{
		CU::Matrix44f& m_Orientation;
		float m_Range;
		float m_Angle;
		CU::Vector2f padding;
	};

			
};

