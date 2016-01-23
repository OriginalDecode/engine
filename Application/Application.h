#pragma once
#include "../Engine/EngineEnums.h"

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
	void Render();

	void OnPause();
	void OnResume();

private:

	Snowblind::CEffect* myEffect;
	Snowblind::CModel* myModel;
	Snowblind::CCamera* myCamera;
	Snowblind::CInstance* myInstance;

};

