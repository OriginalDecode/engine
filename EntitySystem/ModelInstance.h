#pragma once
#include <Math/Matrix/Matrix44.h>
#include "engine_shared.h"
class BaseModel;
class Material;
class Surface;
namespace graphics
{
	class RenderContext;
};

class ModelInstance
{
public:
	ModelInstance() = default;

	void SetModel(BaseModel* const model);
	void SetOrientation(const CU::Matrix44f orientation);
	void SetMaterial(Material* const pMaterial);
	void Draw(const graphics::RenderContext& rc) const;
	void SetSurface(Surface* const pSurface);

	void UpdateMaterial();

	BaseModel* GetModel() { return m_Model; }

	const CU::Matrix44f& GetOrientation() const { return  m_Orientation; }

	u64 GetMaterialKey() const;

private:
	Surface* m_Surface = nullptr;
	BaseModel* m_Model = nullptr;
	Material* m_Material = nullptr;
	CU::Matrix44f m_Orientation;

};