#include "stdafx.h"
#include "InstancingManager.h"
#include <Engine/Material.h>
#include <typeinfo>

bool InstancingManager::FindInstanceObject(u64 key)
{
	if (m_InstanceObjects.find(key) != m_InstanceObjects.end())
		return true;

	return false;
}

bool InstancingManager::FindModel(u64 key) const
{


	return false;
}

void InstancingManager::AddInstanceObject(InstanceObject instance_object)
{
	const u64 material_key = instance_object.m_Material->GetKey();
	const u64 model_key = instance_object.m_Model->GetKey();

	if (m_InstanceObjects.find(material_key) == m_InstanceObjects.end())
	{
		m_InstanceObjects.emplace(material_key, std::map<u64, InstanceObject>());
	}

	auto it = m_InstanceObjects.find(material_key);
	if (it->second.find(model_key) == it->second.end())
	{
		std::map<u64, InstanceObject>& list = m_InstanceObjects.at(material_key);
		list.emplace(model_key, instance_object);
	}
}

// const InstanceObject& InstancingManager::GetInstanceObject(u64 material_key, u64 model_key)
// {
// 	/*auto it = m_InstanceObjects.find(key);
// 	if (it != m_InstanceObjects.end())
// 		return it->second;
// 		*/
// 	DL_ASSERT("Instance not found");
// 	return InstanceObject();
// }

void InstancingManager::AddGPUDataToInstance(u64 material_key, u64 model_key, GPUModelData data)
{
	auto it = m_InstanceObjects.find(material_key);
	if (it == m_InstanceObjects.end())
		return;

	auto model_it = it->second.find(model_key);
	if (model_it == it->second.end())
		return;

	model_it->second.m_GPUData.Add(data);

}

void InstancingManager::DoInstancing(const graphics::RenderContext& rc, bool shadowing)
{
	for (auto it = m_InstanceObjects.begin(); it != m_InstanceObjects.end(); it++)
	{
		for (auto obj = it->second.begin(); obj != it->second.end(); obj++)
		{
			InstanceObject& instance = obj->second;

			if (instance.m_GPUData.Empty())
				continue;

			RefPointer<Model> pModel = instance.m_Model;
			if (!shadowing)
				instance.m_Material->Use(pModel->GetEffect());

			for (const GPUModelData& data : instance.m_GPUData)
			{
				pModel->AddInstanceData(data);
			}

			if (!shadowing)
				pModel->RenderInstanced(rc);
			else
				pModel->ShadowRenderInstanced(rc);

		}
		//instance.m_GPUData.RemoveAll();
	}
}

void InstancingManager::EndFrame()
{
	for (auto it = m_InstanceObjects.begin(); it != m_InstanceObjects.end(); it++)
	{
		for (auto obj = it->second.begin(); obj != it->second.end(); obj++)
		{
			InstanceObject& instance = obj->second;
			if (instance.m_GPUData.Empty())
				continue;

			instance.m_GPUData.RemoveAll();
		}
	}
}
