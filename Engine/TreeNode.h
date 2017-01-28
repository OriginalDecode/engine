#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include "NodeEntityManager.h"
namespace Hex
{
	class Synchronizer;
};

typedef CU::GrowingArray<Entity> EntityList;
class TreeNode
{
public:
typedef CU::GrowingArray<TreeNode*> NodeList;
	TreeNode() = default;

	void Initiate(float halfwidth);

	void AddChild(TreeNode& child_node);
	void AddParent(TreeNode& parent_node);
	void AddEntity(TreeDweller* dweller);
	void AddEntity(TreeDweller* dweller, s32 node);
	void SetPosition(CU::Vector3f position);
	void Update(float dt);
	TreeNode& GetChildByIndex(s32 index);
	void TogglePause() { m_Paused = !m_Paused; }
private:
	bool m_Paused = false;
	void RenderBox();
	NodeEntityManager m_NodeEntityManager;
	CU::Vector3f m_CenterPosition;
	float m_HalfWidth = 0.f;
	TreeNode* m_Parent = nullptr;
	NodeList m_Children;
	EntityList m_Entities;
	Hex::Synchronizer* m_Synchronizer = nullptr;

};