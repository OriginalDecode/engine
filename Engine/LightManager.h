#pragma once
#include <DataStructures/GrowingArray.h>


class SpotLight;
class PointLight;
class DirectionalLight;





class LightManager
{
public:
	LightManager() = default;

	int32 RegisterSpotlight();
	int32 RegisterPointlight();
	int32 RegisterDirectionalLight();

	SpotLight* GetSpotlight(int32 index);
	PointLight* GetPointlight(int32 index);
	DirectionalLight* GetDirectionalLight(int32 index);

	//Area light?
	//Light cube? This is a sort of light volume thing, it emits light and looks pretty.
private:

	CU::GrowingArray<SpotLight*> m_Spotlights;
	CU::GrowingArray<DirectionalLight*> m_DirectionalLights;
	CU::GrowingArray<PointLight*> m_Pointlights;


	//contains lists of lights


};

