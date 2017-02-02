#pragma once
#include "Octree.h"
namespace Hex
{
	class Synchronizer;
};
#include <TimeManager.h>

class World
{
public:
	World() = default;
	void Initiate(CU::Vector3f position);
	void Update(float dt);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);


	//void ToggleNode(s32 index);

	float getTime(int index);

private:

	float sec = 10.f;

	int timer_index = 0;
	float highest = 0.f;
	CU::Vector3f m_WorldPos;
	Hex::Synchronizer* m_Synchronizer;
	Octree m_Octree;
	CU::TimeManager m_Timer;
};

