#pragma once
#include "snowblind_shared.h"
#include <BaseComponent.h>
#include <DataStructures/GrowingArray.h>

#define COMPONENT( x ) \
	private: \
		class x##Component; \
		x##Component* m_##x; \
		\
	public: \
		x##Component* Get##x() const \
		{\
			 return m_##x; \
		}\
		void Set##x(x##Component* component)\
		{\
			m_##x = component;\
		}
/*
	How do I create all the components? Reflection?

*/
struct ComponentPair
{
	BaseComponent* m_Component = nullptr;
	s32 m_ID = -1;
};

typedef CU::GrowingArray<ComponentPair> ComponentList;

class TreeDweller
{
public:
	TreeDweller() = default;
	void Initiate(Entity entity);

	template <typename T>
	void AddComponent(T* component);


	Entity GetEntity() const;
	const ComponentList& GetComponentPairList() const;

private:
	Entity m_Entity;
	ComponentList m_ComponentList;
	/* a list with every thinkable component(?) does not feel viable */
	//COMPONENT(Render);


};

template <typename T>
void TreeDweller::AddComponent(T* component)
{
	ComponentPair pair;
	pair.m_Component = component;
	pair.m_ID = CTypeID<T>::GetID();
	m_ComponentList.Add(pair);
}


