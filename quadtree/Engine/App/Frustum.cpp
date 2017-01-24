#include "Frustum.h"
#include "TreeDweller.h"
#include "Misc\Global.h"
#include "Synchronizer.h"
#include "Misc\Megaton.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::Initiate()
{
	myFarPlane = 300.f;

	myFrustum.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));
	myFrustum.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));
	myFrustum.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));

	myLineBox.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));
	myLineBox.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));
	myLineBox.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));
	myLineBox.AddLine(CU::Line<float>(Vector2f(0, 0), myFarPlane));

	ViewDirection({ 0.f, 0.f });

}

const bool Frustum::IsInside(TreeDweller* anObject) const
{
	if (myLineBox.Inside(anObject->GetPosition(), anObject->GetCircle().GetRadius()) == true)
	{
		if (myFrustum.Inside(anObject->GetPosition(), anObject->GetCircle().GetRadius()) == true)
			return true;

		return false;
	}
	return false;
}

void Frustum::ViewDirection(const Vector2f& aPosition)
{
	Vector2f direction = aPosition - myPosition;

	myAngle = atan2(direction.myY, direction.myX);
	Update();
}

void Frustum::Update()
{
	myFrustum.myLines[0].InitWith2Points(Vector2f(myPosition.myX + myFarPlane * cos(myAngle + PI *0.25), myPosition.myY + myFarPlane * sin(myAngle + PI *0.25)), myPosition);
	myFrustum.myLines[1].InitWith2Points(Vector2f(myPosition.myX + myFarPlane * cos(myAngle - PI *0.25), myPosition.myY + myFarPlane * sin(myAngle - PI *0.25)), myFrustum.myLines[0].myStart);
	myFrustum.myLines[2].InitWith2Points(myPosition, myFrustum.myLines[1].myStart);

	UpdateAABB();
}

void Frustum::UpdateAABB()
{
	myBoundingBox.SetMinPosition({ MIN(myFrustum.myLines[0].myStart.x, myFrustum.myLines[1].myStart.x), MIN(myFrustum.myLines[0].myStart.y, myFrustum.myLines[1].myStart.y) });

	myBoundingBox.SetMinPosition({ MIN(myBoundingBox.GetMinPosition().x, myFrustum.myLines[2].myStart.x), MIN(myBoundingBox.GetMinPosition().y, myFrustum.myLines[2].myStart.y) });

	myBoundingBox.SetMaxPosition({ MAX(myFrustum.myLines[0].myStart.x, myFrustum.myLines[1].myStart.x), MAX(myFrustum.myLines[0].myStart.y, myFrustum.myLines[1].myStart.y) });
	myBoundingBox.SetMaxPosition({ MAX(myBoundingBox.GetMaxPosition().x, myFrustum.myLines[2].myStart.x), MAX(myBoundingBox.GetMaxPosition().y, myFrustum.myLines[2].myStart.y) });



	myBoundingBox.SetCenterPosition(myBoundingBox.GetMinPosition() + ((myBoundingBox.GetMaxPosition() - myBoundingBox.GetMinPosition()) *0.5f));
	myBoundingBox.SetSize({ (myBoundingBox.GetMaxPosition().x - myBoundingBox.GetMinPosition().x) *0.5f, (myBoundingBox.GetMaxPosition().y - myBoundingBox.GetMinPosition().y) *0.5f });

	const Vector2f topLeft(myBoundingBox.GetMinPosition().x, myBoundingBox.GetMinPosition().y);
	const Vector2f botRight(myBoundingBox.GetMaxPosition().x, myBoundingBox.GetMaxPosition().y);

	myLineBox.myLines[0].InitWith2Points(topLeft, { botRight.x, topLeft.y });
	myLineBox.myLines[1].InitWith2Points({ botRight.x, topLeft.y }, botRight);
	myLineBox.myLines[2].InitWith2Points(botRight, { topLeft.x, botRight.y });
	myLineBox.myLines[3].InitWith2Points({ topLeft.x, botRight.y }, topLeft);
}

void Frustum::MoveTo(const Vector2f& aPosition)
{
	myPosition = aPosition;
	Update();
}

void Frustum::Render(Synchronizer& aSynchronizer)
{
	
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myFrustum.myLines[0].myStart.x, myFrustum.myLines[0].myStart.y), Vector2f(myFrustum.myLines[1].myStart.x, myFrustum.myLines[1].myStart.y), 0xFFFF5555));
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myFrustum.myLines[1].myStart.x, myFrustum.myLines[1].myStart.y), Vector2f(myFrustum.myLines[2].myStart.x, myFrustum.myLines[2].myStart.y), 0xFFFF5555));
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myFrustum.myLines[2].myStart.x, myFrustum.myLines[2].myStart.y), Vector2f(myFrustum.myLines[0].myStart.x, myFrustum.myLines[0].myStart.y), 0xFFFF5555));

	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myLineBox.myLines[0].myStart.x, myLineBox.myLines[0].myStart.y), Vector2f(myLineBox.myLines[1].myStart.x, myLineBox.myLines[1].myStart.y), 0xFFFFFFFF));
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myLineBox.myLines[1].myStart.x, myLineBox.myLines[1].myStart.y), Vector2f(myLineBox.myLines[2].myStart.x, myLineBox.myLines[2].myStart.y), 0xFFFFFFFF));
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myLineBox.myLines[2].myStart.x, myLineBox.myLines[2].myStart.y), Vector2f(myLineBox.myLines[3].myStart.x, myLineBox.myLines[3].myStart.y), 0xFFFFFFFF));
	aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(myLineBox.myLines[3].myStart.x, myLineBox.myLines[3].myStart.y), Vector2f(myLineBox.myLines[0].myStart.x, myLineBox.myLines[0].myStart.y), 0xFFFFFFFF));

}