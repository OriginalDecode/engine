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
		TEXT,
		MODEL,
		POINTLIGHT,
	};
	SRenderCommand();
	SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType = eType::TEXT);
	SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType);
	SRenderCommand(Snowblind::CPointLight* aPointLight, const eType& aType);

	Snowblind::CInstance* myInstance;
	Snowblind::CPointLight* myPointLight;
	std::string myTextToPrint;
	CU::Vector3f myPosition;
	eType myType;

};

