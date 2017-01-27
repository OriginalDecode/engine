#include "stdafx.h"
#include "TreeDweller.h"

void TreeDweller::Initiate(Entity entity)
{
	m_Entity = entity;
}

Entity TreeDweller::GetEntity() const
{
	return m_Entity;
}

const ComponentList& TreeDweller::GetComponentPairList() const
{
	return m_ComponentList;
}

