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

	//Area light?
	//Light cube? This is a sort of light volume thing, it emits light and looks pretty.
private:

	CU::GrowingArray<SpotLight*> m_Spotlights;


	//contains lists of lights


};

