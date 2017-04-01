#pragma once
#include "Octree.h"
#include <TimeManager.h>


class Synchronizer;
class World
{
public:
	World() = default;

	void Initiate(CU::Vector3f position);
	void CleanUp();
	
	void Update(float dt);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);
	void AddDweller(TreeDweller* dweller);
private:

	CU::GrowingArray<TreeDweller*> m_WorldDwellers;


	float gettime(int index);
	CU::Vector3f m_WorldPos;
	Synchronizer* m_Synchronizer;
	Octree m_Octree;
	CU::TimeManager m_Timer;
	int timer_index = 0;
	float update_time = 1.f;
	float highest = 0.f;
};

