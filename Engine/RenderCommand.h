#pragma once


namespace Snowblind
{
	class CInstance;
	class CPointLight;
}


struct SRenderCommand
{
	enum class eType
	{
		MODEL,
		POINTLIGHT,
	};
	SRenderCommand();
	SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType);
	SRenderCommand(Snowblind::CPointLight* aPointLight, const eType& aType);
	Snowblind::CInstance* myInstance;
	Snowblind::CPointLight* myPointLight;
	CU::Vector3f myPosition;
	eType myType;

};

