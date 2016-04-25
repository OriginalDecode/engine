#pragma once
#include "../CommonLib/DataStructures/GrowingArray.h"
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif


namespace CommonUtilities
{
	class TimeManager;
}


namespace Snowblind
{
	class CEffect;
	class CModel;
	class CCamera;
	class CInstance;
	class CScene;
	class CFontManager;
	class CSprite;
	class CText;
	class CEngine;
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
	void OnInactive();
	void OnActive();

private:
	void UpdateInput(float aDeltaTime);

	Snowblind::CEffect* myEffect;
	Snowblind::CModel* myModel;
	Snowblind::CModel* myTexturedModel;
	Snowblind::CInstance* myInstance;

	Snowblind::CSprite* mySprite;
	Snowblind::CSprite* mySprite2;

	Snowblind::CText* myText;
	Snowblind::CText* myText2;
	Snowblind::CText* myTextTime;



	Snowblind::CCamera* myCamera;
	Snowblind::CCamera* my2DCamera;

	Snowblind::CScene* myWorldScene;
	Snowblind::CScene* my2DScene;

	Snowblind::CEngine* myEngine;

};

