#include "stdafx.h"
#include "World.h"
#include <Vector.h>
#include "Synchronizer.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

void World::Initiate(CU::Vector3f position)
{
	m_WorldPos = position;
	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	m_Octree.Initiate(position, 1024);
	m_WorldDwellers.Init(16);
}

void World::CleanUp()
{
	m_Octree.OnExit();
	m_WorldDwellers.DeleteAll();
}

void World::Update(float dt, bool paused)
{
	m_Octree.Update(dt, paused);
}

void World::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	m_Octree.AddDwellers(dwellers);


	for (TreeDweller* dweller : dwellers)
	{
		m_WorldDwellers.Add(dweller);
	}
}

void World::AddDweller(TreeDweller* dweller)
{
	m_Octree.AddDweller(dweller);
	m_WorldDwellers.Add(dweller);
}
