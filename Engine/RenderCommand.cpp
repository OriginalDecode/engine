#include "stdafx.h"
#include "RenderCommand.h"



SRenderCommand::SRenderCommand()
{
}

//SRenderCommand::SRenderCommand(Snowblind::CPointLight* aPointLight, const eType& aType)
//	: myPointLight(aPointLight)
//	, myType(aType)
//{
//}

SRenderCommand::SRenderCommand(const std::string aString, const CU::Math::Vector2<float>& aPosition, const eType& aType /*= eType::TEXT*/)
	: myTextToPrint(aString)
	, myPosition(aPosition)
	, myType(aType)
	, myCommandType(eCommandType::e2D)
{
}

SRenderCommand::SRenderCommand(Snowblind::CInstance* anInstance, const CU::Vector3f& aPosition, const eType& aType)
	: myInstance(anInstance)
	, myPosition(aPosition)
	, myType(aType)
	, myCommandType(eCommandType::e3D)
{
}

SRenderCommand::SRenderCommand(Snowblind::CPointLight* aPointLight, const CU::Vector3f& aPosition, const CU::Vector3f& aColor, const eType& aType)
	: myPointLight(aPointLight)
	, myPosition(aPosition)
	, myColor(aColor)
	, myType(aType)
	, myCommandType(eCommandType::LIGHT)
{
}
