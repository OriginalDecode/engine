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
	void AddDweller(TreeDweller* dweller);
private:
	CU::Vector3f m_WorldPos;
	Hex::Synchronizer* m_Synchronizer;
	Octree m_Octree;
};

