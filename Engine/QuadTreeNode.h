#pragma once
#include <engine/engine_shared.h>
#include <DataStructures/StaticArray.h>
#include <Math/Matrix/Matrix.h>
#include <Engine/AABB.h>
class QuadTreeNode
{
public:
	QuadTreeNode(float halfwidth, CU::Vector4f pos);
	QuadTreeNode(float halfwidth, CU::Vector4f pos, int32 index);
	~QuadTreeNode();
	void Update(const CU::Matrix44f& camera_pos);

	void Insert(CU::Vector4f pos);
private:
	void CreateNode(CU::Vector4f pos, int32 index);
	void MoveUp(CU::Vector4f pos);
	int32 m_Index = 0;
	
	cl::AABB m_AABB;

	float m_HalfWidth;
	CU::Vector4f m_Position;

	QuadTreeNode* m_Parent = nullptr;
	CU::StaticArray<QuadTreeNode*, 4> m_Children;
};

