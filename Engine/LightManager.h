#pragma once
#include <DataStructures/GrowingArray.h>


class SpotLight;
class PointLight;
class DirectionalLight;





class LightManager
{
public:
	LightManager() = default;

	s32 RegisterSpotlight();
	s32 RegisterPointlight();
	s32 RegisterDirectionalLight();

	SpotLight* GetSpotlight(s32 index);
	PointLight* GetPointlight(s32 index);
	DirectionalLight* GetDirectionalLight(s32 index);

	//Area light?
	//Light cube? This is a sort of light volume thing, it emits light and looks pretty.
private:

	CU::GrowingArray<SpotLight*> m_Spotlights;
	CU::GrowingArray<DirectionalLight*> m_DirectionalLights;
	CU::GrowingArray<PointLight*> m_Pointlights;


	//contains lists of lights


};

