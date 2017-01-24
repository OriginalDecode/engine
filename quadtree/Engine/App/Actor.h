#pragma once
#include "Misc\Global.h"
#include <hgesprite.h>
#include "Circle.h"
#include "Frustum.h"
class Synchronizer;
class Actor
{
public:
	Actor();
	~Actor();

	void Initiate();
	void Update();
	
	void MoveTo(Vector2f aPosition);
	void Render(Synchronizer& aSynchronizer);

	void LookAt(Vector2f aDirection);

	inline const Vector2f& GetPosition() const;
	inline const float& GetRange() const;
	inline const Circle& GetCircle() const;
	inline Frustum& GetFrustum();
private:

	Vector2f myPosition;
	float myRange;
	HTEXTURE myTexture;
	hgeSprite* mySprite;
	Frustum myFrustum;
	Circle myCircle;

};

inline const Vector2f& Actor::GetPosition() const
{
	return myPosition;
}

inline const float& Actor::GetRange() const
{
	return myRange;
}

inline const Circle& Actor::GetCircle() const
{
	return myCircle;
}

inline Frustum& Actor::GetFrustum()
{
	return myFrustum;
}