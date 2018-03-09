#include "stdafx.h"
#include "InstancingManager.h"
#include <Engine/Material.h>
#include <typeinfo>
#include <Engine/DX11Context.h>
#include <Engine/DX11Device.h>


ID3D11Query* g_QueryStart = nullptr;
ID3D11Query* g_QueryEnd = nullptr;
InstancingManager::InstancingManager()
{
	D3D11_QUERY_DESC desc;
	desc.Query = D3D11_QUERY_TIMESTAMP;
	desc.MiscFlags = 0;

	HRESULT hr = static_cast<graphics::DX11Device&>(Engine::GetAPI()->GetDevice()).GetDevice()->CreateQuery(&desc, &g_QueryStart);
	assert(hr == S_OK);

	hr = static_cast<graphics::DX11Device&>(Engine::GetAPI()->GetDevice()).GetDevice()->CreateQuery(&desc, &g_QueryEnd);
	assert(hr == S_OK);
}

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
	ID3D11DeviceContext* ctx = static_cast<ID3D11DeviceContext*>(static_cast<graphics::DX11Context&>(rc.GetContext()).GetContext());
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
			{
#ifdef _PROFILE
				ctx->End(g_QueryStart);
				while (S_OK != ctx->GetData(g_QueryStart, &instance.m_Start, sizeof(UINT64), 0))
				{
				}
#endif
				pModel->RenderInstanced(rc);
#ifdef _PROFILE
				ctx->End(g_QueryEnd);
				while (S_OK != ctx->GetData(g_QueryEnd, &instance.m_End, sizeof(UINT64), 0))
				{
				}
#endif
			}
			else
			{
#ifdef _PROFILE
				ctx->End(g_QueryStart);
				while (S_OK != ctx->GetData(g_QueryStart, &instance.m_Start, sizeof(UINT64), 0))
				{
				}
#endif
				pModel->ShadowRenderInstanced(rc);
#ifdef _PROFILE
				ctx->End(g_QueryEnd);
				while (S_OK != ctx->GetData(g_QueryEnd, &instance.m_End, sizeof(UINT64), 0))
				{
				}
#endif
			}
		}
	}
}

void InstancingManager::EndFrame()
{
#ifdef _DEBUG
	ID3D11DeviceContext* ctx = static_cast<ID3D11DeviceContext*>(static_cast<graphics::DX11Context&>(Engine::GetAPI()->GetContext()).GetContext());
	graphics::DirectX11* api = static_cast<graphics::DirectX11*>(Engine::GetAPI());
	UINT64 freq = api->GetFrequency();
	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	float total = 0.f;
	for (auto it = m_InstanceObjects.begin(); it != m_InstanceObjects.end(); it++)
	{
		for (auto obj = it->second.begin(); obj != it->second.end(); obj++)
		{
			InstanceObject& instance = obj->second;

			float time = 0;
			if (freq != 0)
				time = (instance.m_End - instance.m_Start) / (float)freq;

			time *= 1000.f;
			std::stringstream ss;
			ss << "Rendering|Instancing|Model|" << time << " ms";
			pDebug->AddTimingObject(ss.str());
			total += time;

		}
	}
	std::stringstream ss;
	ss << "Rendering|Instancing|Model|Total : " << total << " ms";
	pDebug->AddTimingObject(ss.str());

#endif



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
