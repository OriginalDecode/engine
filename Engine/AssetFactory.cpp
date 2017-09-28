#include "stdafx.h"
#include "AssetFactory.h"
#include <Engine/ModelImporter.h>

AssetFactory::AssetFactory()
{
	m_ModelImporter = new CModelImporter;
}

void AssetFactory::LoadModel(BaseModel* pModel, const cl::HashString& filepath, const cl::HashString& effectpath, bool thread /*= true*/)
{
	//m_ModelImporter->LoadModel()
}
