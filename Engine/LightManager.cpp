#include "stdafx.h"
#include "LightManager.h"
#include <Engine/PointLight.h>
#include <Engine/SpotLight.h>
#include <Engine/DirectionalLight.h>


s32 LightManager::RegisterSpotlight()
{
	SpotLight* sl = new SpotLight;
	m_Spotlights.Add(sl);
	return m_Spotlights.Size() - 1;
}

s32 LightManager::RegisterPointlight()
{
	PointLight* pl = new PointLight;
	m_Pointlights.Add(pl);
	return m_Pointlights.Size() - 1;
}

s32 LightManager::RegisterDirectionalLight()
{
	DirectionalLight* dl = new DirectionalLight;
	m_DirectionalLights.Add(dl);
	return m_DirectionalLights.Size() - 1;
}

SpotLight* LightManager::GetSpotlight(s32 index)
{
	return m_Spotlights[index];
}

PointLight* LightManager::GetPointlight(s32 index)
{
	return m_Pointlights[index];
}

DirectionalLight* LightManager::GetDirectionalLight(s32 index)
{
	return m_DirectionalLights[index];
}
