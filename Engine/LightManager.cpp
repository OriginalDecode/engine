#include "stdafx.h"
#include "LightManager.h"
#include <Engine/PointLight.h>
#include <Engine/SpotLight.h>
#include <Engine/DirectionalLight.h>


int32 LightManager::RegisterSpotlight()
{
	SpotLight* sl = new SpotLight;
	m_Spotlights.Add(sl);
	return m_Spotlights.Size() - 1;
}

int32 LightManager::RegisterPointlight()
{
	PointLight* pl = new PointLight;
	m_Pointlights.Add(pl);
	return m_Pointlights.Size() - 1;
}

int32 LightManager::RegisterDirectionalLight()
{
	DirectionalLight* dl = new DirectionalLight;
	m_DirectionalLights.Add(dl);
	return m_DirectionalLights.Size() - 1;
}

SpotLight* LightManager::GetSpotlight(int32 index)
{
	return m_Spotlights[index];
}

PointLight* LightManager::GetPointlight(int32 index)
{
	return m_Pointlights[index];
}

DirectionalLight* LightManager::GetDirectionalLight(int32 index)
{
	return m_DirectionalLights[index];
}
