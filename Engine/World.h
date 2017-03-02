#pragma once
#include "Octree.h"
#include <TimeManager.h>


class Synchronizer;
class World
{
public:
	World() = default;
	void Initiate(CU::Vector3f position);
	void Update(float dt);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);
	void AddDweller(TreeDweller* dweller);
private:
	CU::Vector3f m_WorldPos;
	Synchronizer* m_Synchronizer;
	Octree m_Octree;
};

