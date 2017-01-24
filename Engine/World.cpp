#include "stdafx.h"
#include "World.h"
#include <Vector.h>

void World::Initiate(CU::Vector3f position)
{
	m_WorldPos = position;
	m_Root.Initiate(256.f);
	m_Root.SetPosition(m_WorldPos);
}

void World::Update(float dt)
{
	m_Root.Update(dt);
}
