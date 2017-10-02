#pragma once


class BaseModel;
class CModelImporter;
class AssetFactory
{
public:
	AssetFactory();

	void LoadModel(BaseModel* pModel, const std::string& filepath, const std::string& effectpath, bool thread = true);


private:
	CModelImporter* m_ModelImporter = nullptr;

};

