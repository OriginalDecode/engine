#pragma once
#include "TreeNode.h"

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


	void ToggleNode(s32 index);

private:
	TreeNode m_Root;
	CU::GrowingArray<TreeNode> tempchildern;


	CU::Vector3f m_WorldPos;
	Hex::Synchronizer* m_Synchronizer;
	
};

