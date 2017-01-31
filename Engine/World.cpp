#include "stdafx.h"
#include "World.h"
#include <Vector.h>
#include "Synchronizer.h"
#include "Engine.h"
#include "EntityManager.h"
void World::Initiate(CU::Vector3f position)
{
	m_WorldPos = position;
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
	m_Octree.Initiate(position, 256.f);

}

void World::Update(float dt)
{
	m_Octree.Update(dt);
}

void World::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	m_Octree.AddDwellers(dwellers);
}

//void World::ToggleNode(s32 index)
//{
//	m_Root.GetChildByIndex(index).TogglePause();
//}