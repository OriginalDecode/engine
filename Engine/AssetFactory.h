#pragma once


class BaseModel;
class CModelImporter;
class AssetFactory
{
public:
	AssetFactory();

	void LoadModel(BaseModel* pModel, const cl::HashString& filepath, const cl::HashString& effectpath, bool thread = true);


private:
	CModelImporter* m_ModelImporter = nullptr;

};

