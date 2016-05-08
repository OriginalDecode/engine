#pragma once
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/Math/Quaternion/Quaternion.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif


namespace CommonUtilities
{
	class TimeManager;
	class ControllerInput;
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
	class CConsole;
	class CSynchronizer;
	class CPointLight;
};

namespace std
{
	class thread;
};


class CApplication
{
public:
	CApplication();
	~CApplication();
	void Initiate(float aWindowWidth, float aWindowHeight);

	void Update();
	void Render(); //Use later

	void OnPause();
	void OnResume();
	void OnInactive();
	void OnActive();
	void OnExit();
	bool HasQuit();

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
	Snowblind::CConsole* myConsole;
	Snowblind::CPointLight* myPointLight;

	Snowblind::CSynchronizer* mySynchronizer;

	CU::Quaternion myPitch;
	CU::Quaternion myYaw;
	CU::Matrix44f myOrientation;
	CU::Math::Vector2<float> myCursorPosition;
	CU::ControllerInput* myController;

	CU::GrowingArray<CU::Vector3f> myPositions;


	std::thread* myLogicThread;

	volatile bool myQuitFlag;
	float myAverageFPS;
	float myAverageFPSToPrint;

};

