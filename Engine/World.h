#pragma once
#include "Octree.h"
#include <CommonLib/Timer/TimeManager.h>


class Synchronizer;
class World
{
public:
	World() = default;

	void Initiate(CU::Vector3f position);
	void CleanUp();
	
	void Update(float dt, bool paused);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);
	void AddDweller(TreeDweller* dweller);
private:

	CU::GrowingArray<TreeDweller*> m_WorldDwellers;
	CU::Vector3f m_WorldPos;
	Synchronizer* m_Synchronizer;
	Octree m_Octree;
};

