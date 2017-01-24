#pragma once
#include "DataStructures\GrowingArray.h"
#include "Math\Line\LineSegment2D.h"
#include "Circle.h"
namespace CU = CommonUtilities;
#define CELL_WIDTH 40
#define CELL_HEIGHT 40
#define GRID_WIDTH 20
#define GRID_HEIGHT 20

#define RADIUS 24.f

struct GridCell;

struct GridDweller
{

	Vector2f position;
	GridCell* occupiedCell;
	Circle circle;
	bool isVisible = false;

	inline void AddCircle();

};

inline void GridDweller::AddCircle()
{
	
	circle.SetCenterPosition(position);
	circle.SetRadius(RADIUS);

}

struct GridCell
{
	
	CU::GrowingArray<GridDweller*> objects;
	inline void Remove(GridDweller* anObject);
	inline void Initiate();
	inline bool SetVisible(const bool aIsVisible);
	bool beingVisited = false;

};

inline void GridCell::Initiate()
{
	objects.Init(4);
}

inline void GridCell::Remove(GridDweller* anObject)
{
	objects.RemoveCyclic(anObject);
}

inline bool GridCell::SetVisible(const bool aIsVisible)
{
	
	for (int i = 0; i < objects.Size(); ++i)
	{
		objects[i]->isVisible = aIsVisible;
	}
	beingVisited = true;

	if (objects.Size() > 0)
		return true;

	return false;
	
}

class Grid
{
public:
	Grid();
	~Grid();

	void Initiate();
	void Insert(GridDweller* anObject);
	void Update(GridDweller* anObject);
	const bool HasDweller(const int anIndex) const;
	const bool Collision(const Intersection::LineSegment2D& aLine, const int anIndex) const;
	const bool CellIsVisible(const int anIndex) const;
	const bool IsCellVisited(const int anIndex) const;
	void ResetVisibility();
	const int Size() const;
	CU::GrowingArray<GridCell*>& GetGrid();




private:

	CU::GrowingArray<GridCell*> myGrid;



};