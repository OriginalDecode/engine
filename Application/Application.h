#pragma once
#include "../Engine/EngineEnums.h"
#include "../CommonLib/DataStructures/GrowingArray.h"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

namespace Snowblind
{
	class CEffect;
	class CModel;
	class CCamera;
	class CInstance;
};

class CApplication
{
public:
	CApplication();
	~CApplication();
	void Initiate(float aWindowWidth, float aWindowHeight);

	bool Update();
	void Render(); //Use later

	void OnPause();
	void OnResume();

private:
	void UpdateInput(float aDeltaTime);


	Snowblind::CEffect* myEffect;
	Snowblind::CModel* myModel;
	Snowblind::CModel* myTexturedModel;


	Snowblind::CCamera* myCamera;



	CU::GrowingArray<Snowblind::CInstance*> myInstances;
	//Snowblind::CInstance* myInstance;

};

