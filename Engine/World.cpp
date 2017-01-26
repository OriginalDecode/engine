#include "stdafx.h"
#include "World.h"
#include <Vector.h>
#include "Synchronizer.h"
#include "Engine.h"
void World::Initiate(CU::Vector3f position)
{
	m_WorldPos = position;
	m_Root.Initiate(256.f);
	m_Root.SetPosition(m_WorldPos);
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
}

void World::Update(float dt)
{
	m_Root.Update(dt);
}
