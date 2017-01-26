#include "stdafx.h"
#include "World.h"
#include <Vector.h>
#include "Synchronizer.h"
#include "Engine.h"
#include "EntityManager.h"
void World::Initiate(CU::Vector3f position)
{
	m_WorldPos = position;
	m_Root.Initiate(256.f);
	m_Root.SetPosition(m_WorldPos);
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();


	const EntityArray& entities = Hex::Engine::GetInstance()->GetEntityManager().GetEntities();
	for (Entity e : entities)
	{
		m_Root.AddEntity(e);
	}


}

void World::Update(float dt)
{
	m_Root.Update(dt);
}
