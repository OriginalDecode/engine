#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include "NodeEntityManager.h"
namespace Hex
{
	class Synchronizer;
};
class TreeDweller;
typedef CU::GrowingArray<Entity> EntityList;
class TreeNode
{
public:
	TreeNode() = default;
	~TreeNode();
	void Initiate(float halfwidth);

	void AddChild(TreeNode* child_node, s32 index);
	void AddParent(TreeNode* parent_node);
	void AddEntity(TreeDweller* dweller);
	void AddEntity(TreeDweller* dweller, s32 node);
	void SetPosition(CU::Vector3f position);
	void Update(float dt);
	TreeNode* GetChildByIndex(s32 index);
	void TogglePause() { m_Paused = !m_Paused; }

	const CU::Vector3f& GetPosition() const { return m_CenterPosition; }
	void SetDepth(s32 depth) { m_Depth = depth; }
	s32 GetDepth() const { return m_Depth; }
	float GetHalfWidth() const { return m_HalfWidth; }

	s32 GetEntityCount() const { return m_Entities.Size(); }
	TreeNode* GetParent() { return m_Parent; }

private:
	bool m_Paused = false;
	void RenderBox();
	NodeEntityManager m_NodeEntityManager;
	CU::Vector3f m_CenterPosition;
	float m_HalfWidth = 0.f;
	TreeNode* m_Parent = nullptr;
	TreeNode* m_Children[8];
	EntityList m_Entities;
	Hex::Synchronizer* m_Synchronizer = nullptr;
	s32 m_Depth;
};