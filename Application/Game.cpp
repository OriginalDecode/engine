#include "Game.h"

#include <EntityManager.h>


#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Line3D.h>

#include <DL_Debug.h>
#include <Engine.h>
#include <Utilities.h>
#include <MousePicker.h>

#include <EngineDefines.h>

bool Game::Initiate()
{
	myEngine = Snowblind::Engine::GetInstance();
	mySynchronizer = myEngine->GetSynchronizer();

	myEngine->LoadLevel("Data/Levels/level_01.json");

	myPicker = new Snowblind::CMousePicker;

	myEngine->ToggleVsync(); //settings
	return true;
}

bool Game::CleanUp()
{
	SAFE_DELETE(myPicker);
	if (myPicker)
		return false;

	myTerrain.RemoveAll();
	if (myTerrain.Size() > 0)
		return false;

	return true;
}
static float speed = 50.f;

void Game::Update(float aDeltaTime)
{
	myFrameCount++;
	myAverageFPS += myEngine->GetFPS();
	myTime -= aDeltaTime;
	if (myTime <= 0.f)
	{
		myFPSToPrint = static_cast<u32>(myAverageFPS / myFrameCount);
		myFrameCount = 0;
		myAverageFPS = 0.f;
		myTime = 1.f;
	}

	std::stringstream ss;
	ss << "FPS : " << myEngine->GetFPS() << "\n" << "Average FPS : " << myFPSToPrint << "\nDeltaTime:" << aDeltaTime << "\n" << Snowblind::Engine::GetInstance()->GetLocalTimeAsString()
		<< "\ncamera_speed_multiplier : " << speed;
	//myEngine->AddDebugText(ss.str());


	mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", pointHit));

	//mySynchronizer->AddRenderCommand(RenderCommand(eType::TERRAIN)); This shouldn't even be a command. This should just be rendered

	myEngine->GetEntityManager()->Update(aDeltaTime); 
}