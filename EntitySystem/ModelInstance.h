#pragma once
#include <Math/Matrix/Matrix44.h>
#include "engine_shared.h"
#include <string>
#include <Engine/BaseModel.h>
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

	~ModelInstance();

	void SetModel(BaseModel* const model);
	void SetOrientation(const CU::Matrix44f orientation);
	void SetMaterial(Material* const pMaterial);
	void Draw(const graphics::RenderContext& rc) const;
	void SetSurface(Surface* const pSurface);

	void UpdateMaterial();

	BaseModel* GetModel() { return m_Model; }

	const CU::Matrix44f& GetOrientation() const { return  m_Orientation; }

	uint64 GetMaterialKey() const;
	void SetMaterialKey(uint64 key);
		
	const std::string& GetModelName() const { return m_Model->GetFileName(); }

private:
	uint64 m_Key;
	Surface* m_Surface = nullptr;
	BaseModel* m_Model = nullptr;
	Material* m_Material = nullptr;
	CU::Matrix44f m_Orientation;

};