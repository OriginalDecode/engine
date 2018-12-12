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

	TreeNodeBase();
	virtual ~TreeNodeBase();

	virtual void Initiate(float halfwidth, Octree* octree) = 0;

	virtual void Update(float dt, bool paused);

	virtual void SetDepth(int32 depth);
	virtual int32 GetDepth() const;

	virtual void AddChild(TreeNodeBase* child_node, int32 index);
	virtual void AddParent(TreeNodeBase* parent);
	virtual TreeNodeBase* GetParent();

	virtual void AddEntity(TreeDweller* dweller);
	virtual void AddEntity(TreeDweller* dweller, int32 node);
	virtual void RemoveEntity(TreeDweller* dweller);

	virtual void SetPosition(CU::Vector3f position);
	virtual const CU::Vector3f& GetPosition() const;

	virtual TreeNodeBase* GetChildByIndex(int32 index);
	virtual bool HasEntities();
	virtual float GetHalfWidth() const;


	virtual bool InsideNode(TreeDweller* dweller);

	virtual void SetMemoryBlockIndex(int32 index);
	virtual int32 GetMemoryBlockIndex();

	void ToggleRenderBox(bool v);

	void RemoveAllDwellers();

	std::string m_Name;
	NodeEntityManager* GetManager() { return m_NodeEntityManager; }
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
	int32 m_Depth = 0;
	int32 m_MemoryBlockIndex = 0;

	int32 m_DwellerCount = 0;




};