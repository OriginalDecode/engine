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

	void SetOrientation(const CU::Matrix44f orientation) override;
	const CU::Matrix44f& GetOrientation() const override;

	void SetPosition(const CU::Vector4f& v) override;
	void SetForward(const CU::Vector4f& d) override;

private:
	void AddSurface(Surface* p) override { delete p; p = nullptr; }
	CU::GrowingArray<LightModel*> m_Children;
			
};

