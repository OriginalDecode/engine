#pragma once
#include <standard_datatype.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Snowblind
{
	class CModel;
	class CEffect;
	class CEngine;
};

namespace CommonUtilities
{
	class TimeManager;
}



class CModelImporter
{
public:
	CModelImporter();
	~CModelImporter();

	Snowblind::CModel* LoadModel(const s8* aFilePath, const s8* aEffectPath);
	void ImportModel(const std::string& aFilepath);

private:
	Snowblind::CModel* CreateModel(/*Data*/ Snowblind::CEffect* anEffect);
	CommonUtilities::TimeManager* myTimeManager;
	Snowblind::CModel* LoadModel(const s8* aFilePath, Snowblind::CEffect* anEffect);
	Snowblind::CEngine* myEngine;
	void FillData(Snowblind::CModel* out, Snowblind::CEffect* anEffect);

	Assimp::Importer importer;
};

