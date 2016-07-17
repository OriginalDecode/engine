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
}

class CEntityManager;

namespace Snowblind
{
	class CEffect;
	class CCamera;
	class CFontManager;
	class CEngine;
	class CSynchronizer;
	class CPointLight;
	class CEmitterInstance;
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
	void OnAltEnter();
	bool HasQuit();

private:
	void UpdateInput(float aDeltaTime);

	Snowblind::CEffect* myEffect;

	Snowblind::CCamera* myCamera;
	Snowblind::CCamera* my2DCamera;

	Snowblind::CEngine* myEngine;
	Snowblind::CPointLight* myPointLight;

	Snowblind::CSynchronizer* mySynchronizer;
	Snowblind::CEmitterInstance* myEmitter;

	CU::Quaternion myPitch;
	CU::Quaternion myYaw;
	CU::Matrix44f myOrientation;
	CU::Math::Vector2<float> myCursorPosition;

	CU::GrowingArray<CU::Vector3f> myPositions;
	
	float myWindowWidth;
	float myWindowHeight;

	std::thread* myLogicThread;

	volatile bool myQuitFlag;
	bool myWindowIsActive = true;
	float myAverageFPS;
	float myAverageFPSToPrint;
	CEntityManager* myEntityManager;

};

