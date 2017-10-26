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
	if (it != m_InstanceObjects.end())
		return it->second;

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

void InstancingManager::AddGPUDataToInstance(u64 key, GPUModelData data)
{
	auto it = m_InstanceObjects.find(key);
	if (it == m_InstanceObjects.end())
		return;
	it->second.m_GPUData.Add(data);
}

void InstancingManager::DoInstancing(const graphics::RenderContext& rc, bool shadowing)
{
	for (auto it = m_InstanceObjects.begin(); it != m_InstanceObjects.end(); it++)
	{

		InstanceObject& instance = it->second;

		if (instance.m_GPUData.Empty())
			continue;

		Model* pModel = instance.m_Model;




		if (!shadowing)
			instance.m_Material->Use(pModel->GetEffect());


		//CU::GrowingArray<GPUModelData>::Copy(pModel->m_GPUData, instance.m_GPUData);


		for (const GPUModelData& data : instance.m_GPUData)
		{
			pModel->AddInstanceData(data);
		}



		if (!shadowing)
			pModel->RenderInstanced(rc);
		else
			pModel->ShadowRenderInstanced(rc);

		instance.m_GPUData.RemoveAll();


	}
}
