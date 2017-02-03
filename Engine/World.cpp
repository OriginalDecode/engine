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
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
	m_Octree.Initiate(position, 256.f);

	m_Timer.CreateTimer();
	m_Timer.CreateTimer();
	m_Timer.CreateTimer();
	m_Timer.CreateTimer();
	m_Timer.CreateTimer();
}

void World::Update(float dt)
{
	CU::Timer& timer = m_Timer.GetTimer(timer_index);
	timer.ClearTime();
	timer.Start();

	m_Octree.Update(dt);

	timer.Update();
	timer.Pause();
	timer_index++;
	if (timer_index > 4)
		timer_index = 0;

	if (sec > 0.f)
	{
		sec -= dt;
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			if (getTime(i) > highest)
				highest = getTime(i);
		}
	}
	std::stringstream ss;
	ss << "0 : " << m_Timer.GetTimer(0).GetTotalTime().GetMilliseconds() << "\n"
		<< "1 : " << m_Timer.GetTimer(1).GetTotalTime().GetMilliseconds() << "\n"
		<< "2 : " << m_Timer.GetTimer(2).GetTotalTime().GetMilliseconds() << "\n"
		<< "3 : " << m_Timer.GetTimer(3).GetTotalTime().GetMilliseconds() << "\n"
		<< "4 : " << m_Timer.GetTimer(4).GetTotalTime().GetMilliseconds() << "\n"
		<< "Highest : " << highest;

	Hex::Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::TEXT, ss.str().c_str(), CU::Vector2f(0.75f, 0.0f)));
	
}

void World::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	m_Octree.AddDwellers(dwellers);
}

float World::getTime(int index)
{
	return m_Timer.GetTimer(index).GetTotalTime().GetMilliseconds();
}

//void World::ToggleNode(s32 index)
//{
//	m_Root.GetChildByIndex(index).TogglePause();
//}
