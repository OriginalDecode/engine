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


	TreeNode node;
	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(128.f,0.f, 128.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(384.f, 0.f, 128.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(128.f, 0.f, 384.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(384.f, 0.f, 384.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());



}

void World::Update(float dt)
{
	m_Root.Update(dt);
}

void World::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	/*for (TreeDweller* dweller : dwellers)
	{
		m_Root.AddEntity(dweller);
	}
*/
	m_Root.AddEntity(dwellers[0], 0);
	m_Root.AddEntity(dwellers[1], 1);
	m_Root.AddEntity(dwellers[2], 2);
	m_Root.AddEntity(dwellers[3], 3);


}

void World::ToggleNode(s32 index)
{
	m_Root.GetChildByIndex(index).TogglePause();
}
