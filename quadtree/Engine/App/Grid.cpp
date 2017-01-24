#include "Grid.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

void Grid::Initiate()
{
	myGrid.Init(GRID_WIDTH*GRID_HEIGHT);
	for (int i = 0; i < myGrid.Capacity(); ++i)
	{
		GridCell* toAdd;
		toAdd = new GridCell;
		toAdd->Initiate();
		myGrid.Add(toAdd);
	}


}

void Grid::Insert(GridDweller* anObject)
{
	int xPos = int(anObject->position.x / CELL_WIDTH);
	int yPos = int(anObject->position.y / CELL_HEIGHT);

	myGrid[xPos + yPos * GRID_WIDTH]->objects.Add(anObject);
	anObject->occupiedCell = myGrid[xPos + yPos * GRID_WIDTH];
}

void Grid::Update(GridDweller* anObject)
{
	anObject->occupiedCell->Remove(anObject);
	Insert(anObject);
}

const bool Grid::HasDweller(const int anIndex) const
{
	if (myGrid[anIndex]->objects.Size() > 0)
	{
		return true;
	}

	return false;

}

const bool Grid::Collision(const Intersection::LineSegment2D& aLine, const int anIndex) const
{

	Vector2f tempVec2(1.f, 1.f);
	for (int i = 0; i < myGrid[anIndex]->objects.Size(); ++i)
	{
		if (CU::LineVsCircle(aLine, myGrid[anIndex]->objects[i]->circle, tempVec2) == true)
		{
			myGrid[anIndex]->objects[i]->isVisible = true;
			return true;
		}
	}

	return false;

}

const bool Grid::CellIsVisible(const int anIndex) const
{
	return myGrid[anIndex]->SetVisible(true);
}

const bool Grid::IsCellVisited(const int anIndex) const
{
	return myGrid[anIndex]->beingVisited;
}

const int Grid::Size() const
{
	return myGrid.Size();
}

void Grid::ResetVisibility()
{
	for (int i = 0; i < myGrid.Size(); ++i)
	{
		myGrid[i]->SetVisible(false);
		myGrid[i]->beingVisited = false;
	}
}

CU::GrowingArray<GridCell*>& Grid::GetGrid()
{
	return myGrid;
}