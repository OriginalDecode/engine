#include "Actor.h"
#include "RenderCommand.h"
#include "Synchronizer.h"
Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Initiate()
{
	myPosition = { 675.f, 450.f };
	HGE* hge = hgeCreate(HGE_VERSION);
	myTexture = hge->Texture_Load("../Data/Assets/seen.png");
	mySprite = new hgeSprite(myTexture, 0.f, 0.f, 48.f, 48.f);
	mySprite->SetHotSpot(24.f, 24.f);
	myRange = 300.f;
	myCircle.SetCenterPosition(myPosition);
	myCircle.SetRadius(myRange);
	myFrustum.Initiate();
	myFrustum.MoveTo(myPosition);
}

void Actor::Update()
{
}

void Actor::MoveTo(Vector2f aPosition)
{
	myPosition = aPosition;
	myCircle.SetCenterPosition(myPosition);
	myFrustum.MoveTo(myPosition);
}


void Actor::Render(Synchronizer& aSynchronizer)
{
	aSynchronizer.AddRenderCommand(RenderCommand(myPosition, mySprite, RenderCommand::eSPACE::SCREEN));
	myFrustum.Render(aSynchronizer);
}

void Actor::LookAt(Vector2f aDirection)
{
	myFrustum.ViewDirection(aDirection);

}