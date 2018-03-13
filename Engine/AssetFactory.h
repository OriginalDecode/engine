#pragma once


class BaseModel;
class CModelImporter;
class Material;
class AssetFactory
{
public:

	//void LoadModel(BaseModel* pModel, const std::string& filepath, const std::string& effectpath, bool thread = true);
	static void Create();
	static void Destroy();
	static AssetFactory& GetInstance() { return *m_Instance; }

	void CreateMaterial(const std::string& filepath, Material* pMaterial);
	void LoadTexture();
	void LoadSprite();
private:
	static AssetFactory* m_Instance; 
	AssetFactory();
	~AssetFactory() {}

};

