#pragma once
#include <Engine/engine_shared.h>
#include <CommonLib/DataStructures/GrowingArray.h>
#include <CommonLib/DataStructures/StaticArray.h>
#include <CommonLib/Math/Vector/Vector.h>

class Octree;
class NodeEntityManager;
class TreeDweller;
class Synchronizer;

class TreeNodeBase
{
public:

	TreeNodeBase() = default;
	virtual ~TreeNodeBase();

	virtual void Initiate(float halfwidth, Octree* octree) = 0;
	virtual void Update(float dt);

	virtual void SetDepth(s32 depth);
	virtual s32 GetDepth() const;

	virtual void AddChild(TreeNodeBase* child_node, s32 index);
	virtual void AddParent(TreeNodeBase* parent);
	virtual TreeNodeBase* GetParent();


	virtual void AddEntity(TreeDweller* dweller);
	virtual void AddEntity(TreeDweller* dweller, s32 node);
	virtual void RemoveEntity(TreeDweller* dweller);

	virtual void SetPosition(CU::Vector3f position);
	virtual const CU::Vector3f& GetPosition() const;

	virtual TreeNodeBase* GetChildByIndex(s32 index);
	virtual bool HasEntities();
	virtual float GetHalfWidth() const;


	virtual bool InsideNode(TreeDweller* dweller);

	virtual void SetMemoryBlockIndex(s32 index);
	virtual s32 GetMemoryBlockIndex();
protected:
	virtual void RenderBox();

	Octree* m_Octree = nullptr;
	Synchronizer* m_Synchronizer;

	TreeNodeBase* m_Parent = nullptr;
	CU::StaticArray<TreeNodeBase*, 8> m_Children;
	NodeEntityManager* m_NodeEntityManager = nullptr;

	CU::GrowingArray<TreeDweller*> m_Dwellers;

	bool m_RenderBox = false;

	float m_HalfWidth = 0.f;
	CU::Vector3f m_CenterPosition;
	s32 m_Depth = 0;
	s32 m_MemoryBlockIndex = 0;


};
