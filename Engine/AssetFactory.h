#pragma once


class BaseModel;
class CModelImporter;
class Material;
class AssetFactory
{
public:
	AssetFactory();

	//void LoadModel(BaseModel* pModel, const std::string& filepath, const std::string& effectpath, bool thread = true);
	void CreateMaterial(const std::string& filepath, Material* pMaterial);
	void LoadTexture();
	void LoadSprite();
private:
	CModelImporter* m_ModelImporter = nullptr;

};

