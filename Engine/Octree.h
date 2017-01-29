#pragma once
#include "snowblind_shared.h"
#include "TreeNode.h"
#include "TreeDweller.h"

class Octree
{
public:
	Octree() = default;

	void Initiate(CU::Vector3f world_position, float world_half_width);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);

	void Update(float dt);
private:

	void InsertDweller(TreeNode* node, TreeDweller* dweller, s32 depth);
	TreeNode* CreateNode(const CU::Vector3f& center, float halfwidth, s32 index);

	CU::Vector3f m_Position;
	float m_HalfWidth = 0.f;
	TreeNode m_Root;
};

