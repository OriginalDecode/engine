#pragma once

class EntityManager;
class InspectorView
{
public:
	InspectorView(EntityManager& em);
	virtual ~InspectorView() { };

	virtual void Update() = 0;

	virtual void SetEntity(Entity e, s32 component_flag);
	

protected:
	Entity m_CurrentEntity;
	s32 m_ComponentFlags;
	EntityManager& m_Manager;


};

InspectorView::InspectorView(EntityManager& em)
	: m_Manager(em)
{
}

void InspectorView::SetEntity(Entity e, s32 component_flag)
{
	m_CurrentEntity = e;
	m_ComponentFlags = component_flag;
}
