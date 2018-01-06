#include "stdafx.h"
#include "InspectorView.h"


InspectorView::InspectorView(EntityManager& em) 
	: m_Manager(em)
{
}

void InspectorView::SetEntity(Entity e, s32 component_flag)
{
	m_CurrentEntity = e;
	m_ComponentFlags = component_flag;
}
