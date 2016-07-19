#pragma once
#include "../CommonLib/Math/Quaternion/Quaternion.h"
#include "../CommonLib/Math/Matrix/Matrix.h"


namespace Snowblind
{
	class CCamera;
	class CEngine;
	class CSynchronizer;
};

namespace std
{
	class thread;
};

class CGame;

class CApplication
{
public:
	CApplication();
	~CApplication();
	void Initiate(float aWindowWidth, float aWindowHeight);

	void Update();

	void OnPause();
	void OnResume();
	void OnInactive();
	void OnActive();
	void OnExit();
	void OnAltEnter();
	bool HasQuit();

private:
	void UpdateInput(float aDeltaTime);

	/* Threading */
	std::thread* myLogicThread;
	volatile bool myQuitFlag = false;

	/* General */
	Snowblind::CEngine* myEngine;
	Snowblind::CSynchronizer* mySynchronizer;

	/* Should be moved to some kind of InputComponent */
	Snowblind::CCamera* myCamera;

	CU::Quaternion myPitch;
	CU::Quaternion myYaw;
	CU::Matrix44f myOrientation;
	CU::Math::Vector2<float> myCursorPosition;
	bool myWindowIsActive = true;


	CGame* myGame;

};

