#pragma once


namespace Snowblind
{
	class CInstance;
}


struct SRenderCommand
{
	enum class eType
	{
		MODEL,
	};
	SRenderCommand();
	SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType);

	Snowblind::CInstance* myInstance;
	CU::Vector3f myPosition;
	eType myType;

};

