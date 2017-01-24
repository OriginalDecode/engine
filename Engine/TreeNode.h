#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
typedef CU::GrowingArray<Entity> EntityList;
class TreeNode
{
public:
typedef CU::GrowingArray<TreeNode*> NodeList;
	TreeNode() = default;

	void Initiate(float halfwidth);

	void AddChild(TreeNode& child_node);
	void AddParent(TreeNode& parent_node);
	void AddEntity(Entity entity);
	void SetPosition(CU::Vector3f position);
	void Update(float dt);
private:
	CU::Vector3f m_CenterPosition;
	float m_HalfWidth = 0.f;
	TreeNode* m_Parent = nullptr;
	NodeList m_Children;
	EntityList m_Entities;


};