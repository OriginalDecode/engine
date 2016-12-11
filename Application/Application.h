#pragma once
#include "../CommonLib/Math/Quaternion/Quaternion.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#include "../CommonLib/Threadpool.h"

namespace Snowblind
{
	class Camera;
	class Engine;
	class Synchronizer;
};

namespace std
{
	class thread;
};

class Game;
class Application
{
public:
	Application() = default;
	
	bool Initiate();

	void Update();

	void OnPause();
	void OnResume();
	void OnInactive();
	void OnActive();
	void OnExit();
	void OnAltEnter();
	bool HasQuit();
	bool CleanUp();
private:
	/* Threading */
	std::thread* myLogicThread;
	volatile bool myQuitFlag = false;

	/* General */
	Snowblind::Engine* myEngine;
	Snowblind::Synchronizer* mySynchronizer;

	/* Should be moved to some kind of InputComponent */
	Snowblind::Camera* myCamera;

	CU::Quaternion myPitch;
	CU::Quaternion myYaw;
	CU::Matrix44f myOrientation;
	float moveSpeed = 15.f;

	Game* myGame;

};

