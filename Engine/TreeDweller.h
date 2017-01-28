#pragma once
#include "snowblind_shared.h"
#include "../EntitySystem/BaseComponent.h"
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
class TreeNode;
struct ComponentPair
{
	BaseComponent* m_Component = nullptr;
	s32 m_ID = -1;
	s32 m_Type = 0;
};

typedef CU::GrowingArray<ComponentPair> ComponentList;

class TreeDweller
{
public:
	enum Components
	{
		TRANSLATION = 1,
		GRAPHICS = 2,
		LIGHT = 4,
		PHYSICS = 8,
		AI = 16,
		NETWORK = 32,
		INPUT = 64,
		DEBUG = 128,
		CAMERA = 256,
	};


	TreeDweller() = default;
	void Initiate(Entity entity);

	template <typename T>
	void AddComponent(T* component, s32 component_flag);


	Entity GetEntity() const;
	ComponentList& GetComponentPairList();

	void SetFirstNode(TreeNode* node) { m_First = node; }
	TreeNode* GetFirstNode() { return m_First; }

	void SetSecondNode(TreeNode* node) { m_Second = node; }
	TreeNode* GetSecondNode() { return m_Second; }

	void SetDepth(s32 depth) { m_Depth = depth; }
	s32 GetDepth() const { return m_Depth; }

	const CU::Vector3f& GetPosition() const;


private:
	Entity m_Entity;
	ComponentList m_ComponentList;
	s32 m_Depth = 0;
	s32 m_ComponentFlag = 0;

	TreeNode* m_First = nullptr;
	TreeNode* m_Second = nullptr;
};

template <typename T>
void TreeDweller::AddComponent(T* component, s32 component_flag)
{
	ComponentPair pair;
	pair.m_Component = component;
	pair.m_ID = CTypeID<BaseComponent>::GetID<T>();
	pair.m_Type = component_flag;
	m_ComponentList.Add(pair);
	m_ComponentFlag |= component_flag;
	
}