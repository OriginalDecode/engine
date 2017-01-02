#pragma once
#include "../CommonLib/Math/Quaternion/Quaternion.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#include "../CommonLib/Threadpool.h"

namespace Hex
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
	Hex::Engine* myEngine;
	Hex::Synchronizer* mySynchronizer;

	/* Should be moved to some kind of InputComponent */
	Hex::Camera* myCamera;

	CU::Quaternion myPitch;
	CU::Quaternion myYaw;
	CU::Matrix44f myOrientation;
	float moveSpeed = 15.f;

	Game* myGame;

};

