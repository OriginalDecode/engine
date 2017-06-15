#pragma once
#include "engine_shared.h"
#include "TreeNode.h"
#include "TreeDweller.h"
#include <TimeManager.h>
#define MAX_DEPTH 2
class Octree
{
	friend TreeNode;
public:
	Octree() = default;

	void Initiate(CU::Vector3f world_position, float world_half_width);

	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);
	void AddDweller(TreeDweller* dweller);
	void Update(float dt);
private:
	CU::TimeManager m_Timer;

	void MoveDown(TreeNode* node, TreeDweller* dweller, s32 depth);
	void InsertDweller(TreeNode* node, TreeDweller* dweller, s32 depth);

	TreeNode* CreateNode(const CU::Vector3f& center, float halfwidth, s32 index, s32 depth);
	void MoveUp(TreeNode* node, TreeDweller* dweller, s32 depth);

	void ToDelete(TreeNode* node);
	CU::GrowingArray<TreeNode*> m_GarbageNodes;
	CU::GrowingArray<TreeNode*> allNodes;


	
	CU::Vector3f m_Position;
	float m_HalfWidth = 0.f;
	TreeNode m_Root;


	int node_count = 0;
};

