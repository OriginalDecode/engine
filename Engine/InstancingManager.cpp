#include "stdafx.h"
#include "InstancingManager.h"
#include <Engine/Material.h>

bool InstancingManager::FindInstanceObject(u64 key)
{
	if (m_InstanceObjects.find(key) != m_InstanceObjects.end())
		return true;

	return false;
}

InstanceObject& InstancingManager::AddInstanceObject(InstanceObject instance_object)
{
	auto it = m_InstanceObjects.find(instance_object.m_Material->GetKey());
	if (it == m_InstanceObjects.end())
		return;

	m_InstanceObjects.insert(std::pair<u64, InstanceObject>(instance_object.m_Material->GetKey(), instance_object));

}

const InstanceObject& InstancingManager::GetInstanceObject(u64 key)
{
	auto it = m_InstanceObjects.find(key);
	if (it != m_InstanceObjects.end())
		return it->second;

	DL_ASSERT("Instance not found");
	return InstanceObject();
}

void InstancingManager::AddOrientationToInstance(u64 key, const CU::Matrix44f& orientation)
{
	auto it = m_InstanceObjects.find(key);
	if (it == m_InstanceObjects.end())
		return;

	it->second.m_Orientations.Add(orientation);
}
