#pragma once
#include "engine_shared.h"
#include "TreeDweller.h"
#include <Timer/TimeManager/TimeManager.h>

#include <Engine/RootTreeNode.h>
#define MAX_DEPTH 4
class Octree
{
	friend class TreeNodeBase;
public:
	Octree() = default;

	void Initiate(CU::Vector3f world_position, float world_half_width);
	void OnExit();


	void AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers);
	void AddDweller(TreeDweller* dweller);
	void Update(float dt, bool paused);
private:
	CU::TimeManager m_Timer;
	bool m_Paused = false;
	void MoveDown(TreeNodeBase* node, TreeDweller* dweller, s32 depth);
	void InsertDweller(TreeNodeBase* node, TreeDweller* dweller, s32 depth);

	TreeNodeBase* CreateNode(const CU::Vector3f& center, float halfwidth, s32 index, s32 depth);
	void MoveUp(TreeNodeBase* node, TreeDweller* dweller, s32 depth);

	void ToDelete(TreeNodeBase* node);
	CU::GrowingArray<TreeNodeBase*> m_GarbageNodes;
	CU::GrowingArray<TreeNodeBase*> allNodes;


	
	CU::Vector3f m_Position;
	float m_HalfWidth = 0.f;
	RootTreeNode m_Root;


	int node_count = 0;


};

