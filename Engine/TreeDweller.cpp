#include "stdafx.h"
#include "TreeDweller.h"
#include "TranslationComponent.h"

void TreeDweller::Initiate(Entity entity, eType type)
{
	m_Type = type;
	m_Entity = entity;
}

Entity TreeDweller::GetEntity() const
{
	return m_Entity;
}

ComponentList& TreeDweller::GetComponentPairList()
{
	return m_ComponentList;
}

const CU::Vector3f TreeDweller::GetPosition() const
{
	for (const ComponentPair& pair : m_ComponentList)
	{
		if (pair.m_Type & TreeDweller::TRANSLATION)
		{
			return static_cast<TranslationComponent*>(pair.m_Component)->GetOrientation().GetPosition();
		}
	}
	return CU::Vector3f();
}

