#pragma once
#include <vector>

namespace Snowblind
{
	class CModel;
	class CEffect;
};

namespace CommonUtilities
{
	class TimeManager;
};

class FBXLoader;
struct ModelData;
class FbxModelData;

class FBXFactory
{
public:
	FBXFactory();
	~FBXFactory();

	Snowblind::CModel* LoadModel(const char* aFilePath, Snowblind::CEffect* anEffect);
	Snowblind::CModel* LoadModel(const char* aFilePath, const char* anEffectPath);

private:
	void FillData(ModelData* someData, Snowblind::CModel* outData, Snowblind::CEffect* anEffect);
	Snowblind::CModel* CreateModel(FbxModelData* someModelData, Snowblind::CEffect* anEffect);

	FBXLoader *myLoader;
	CU::TimeManager* myTimeManager;
	short myLoadTimer;
	float myLoadTime;


	struct FBXData
	{
		FbxModelData* myData;
		std::string myPath;
	};
	std::vector<FBXData*> myFBXData;
};