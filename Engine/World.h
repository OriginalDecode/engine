#pragma once
#include "Octree.h"
namespace Hex
{
	class Synchronizer;
};

class World
{
public:
	World() = default;
	void Initiate(CU::Vector3f position);
	void Update(float dt);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);


	//void ToggleNode(s32 index);

private:
	CU::Vector3f m_WorldPos;
	Hex::Synchronizer* m_Synchronizer;
	Octree m_Octree;
	
};

