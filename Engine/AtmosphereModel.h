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

	void Render(const graphics::RenderContext& rc) override;

	void AddChild(AtmosphereModel* child);
	void SetOrientation(const CU::Matrix44f& orientation);

private:
	void AddSurface(Surface* p) override { delete p; p = nullptr; }

	CU::GrowingArray<AtmosphereModel*> m_Children;

};

