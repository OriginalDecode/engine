#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include "NodeEntityManager.h"
namespace Hex
{
	class Synchronizer;
};
class Octree;
class TreeDweller;
typedef CU::GrowingArray<Entity> EntityList;
class TreeNode
{
public:
	TreeNode() = default;
	~TreeNode();
	void Initiate(float halfwidth, Octree* octree);

	void AddChild(TreeNode* child_node, s32 index);
	void AddParent(TreeNode* parent_node);
	void AddEntity(TreeDweller* dweller);
	void AddEntity(TreeDweller* dweller, s32 node);
	void RemoveEntity(TreeDweller* dweller);

	void SetPosition(CU::Vector3f position);
	void Update(float dt);
	TreeNode* GetChildByIndex(s32 index);
	void TogglePause() { m_Paused = !m_Paused; }

	const CU::Vector3f& GetPosition() const { return m_CenterPosition; }
	void SetDepth(s32 depth) { m_Depth = depth; }
	s32 GetDepth() const { return m_Depth; }
	float GetHalfWidth() const { return m_HalfWidth; }

	s32 GetEntityCount();
	TreeNode* GetParent() { return m_Parent; }
	bool SubNodeContainsDwellers();
	bool InsideNode(TreeDweller* dweller);
private:
	Octree* m_Octree = nullptr;
	bool m_Paused = false;
	void RenderBox();

	float m_HalfWidth = 0.f;

	TreeNode* m_Parent = nullptr;
	TreeNode* m_Children[8];

	NodeEntityManager m_NodeEntityManager;
	CU::GrowingArray<TreeDweller*> m_Dwellers;
	CU::Vector3f m_CenterPosition;

	Hex::Synchronizer* m_Synchronizer = nullptr;
	s32 m_Depth;
};