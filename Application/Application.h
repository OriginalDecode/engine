#pragma once
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
	class CScene;
	class CFontManager;
	class CSprite;
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
	Snowblind::CInstance* myInstance;

	Snowblind::CFontManager* myFontManager;
	Snowblind::CSprite* mySprite;

	Snowblind::CCamera* myCamera;
	Snowblind::CCamera* my2DCamera;

	Snowblind::CScene* myWorldScene;
	Snowblind::CScene* my2DScene;

};

