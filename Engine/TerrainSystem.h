#pragma once
#include "QuadTree.h"
#include "engine_shared.h"
class TerrainSystem
{
public:
	TerrainSystem();

	void Update();
private:
	QuadTree m_Tree;

};

