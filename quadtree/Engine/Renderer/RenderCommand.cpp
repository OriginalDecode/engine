#include "RenderCommand.h"

//Initierings listor... fy fan för dem.

RenderCommand::RenderCommand(const Vector2f& aPosition, hgeSprite* aSprite, eSPACE aSpace) :
myPosition(aPosition),
mySprite(aSprite),
myType(eTYPE::SPRITE),
mySpace(aSpace)
{
}
RenderCommand::RenderCommand(const Vector2f& aPosition, float aRot, Vector2f& aScale, hgeSprite* aSprite, eSPACE aSpace) :
myPosition(aPosition),
myRotation(aRot),
myScale(aScale),
mySprite(aSprite),
myType(eTYPE::SPRITE_EX),
mySpace(aSpace)
{
}
RenderCommand::RenderCommand(const Vector2f& aPosition, const char* aText, const int anAlignment, DWORD aColour, eSPACE aSpace):
myPosition(aPosition),
myText(aText),
myTextAlignment(anAlignment),
myColour(aColour),
myType(eTYPE::TEXT),
mySpace(aSpace)
{
}
RenderCommand::RenderCommand(const Vector2f& aPosition, std::string aText, const int anAlignment, DWORD aColour, eSPACE aSpace) :
myPosition(aPosition),
myText(aText),
myTextAlignment(anAlignment),
myColour(aColour),
myType(eTYPE::TEXT),
mySpace(aSpace)
{
}
RenderCommand::RenderCommand(std::string aText, const int anAlignment, DWORD aColour, eSPACE aSpace) :
myText(aText),
myTextAlignment(anAlignment),
myColour(aColour),
myType(eTYPE::FORCEDTEXT),
mySpace(aSpace)
{
}


RenderCommand::RenderCommand(const Vector2f& aPosition, const float aRadius, const int aSegmentAmount, DWORD aColour, eSPACE aSpace) :
myPosition(aPosition),
myRadius(aRadius),
mySegmentAmount(aSegmentAmount),
myColour(aColour),
myType(eTYPE::DEBUGCIRCLE),
mySpace(aSpace)
{
}

RenderCommand::RenderCommand(const Vector2f& aPosition, const float aWidth, const float aHeight, int aPointCount, DWORD aColour, eSPACE aSpace) :
myPosition(aPosition),
myWidth(aWidth),
myHeight(aHeight),
myColour(aColour),
myPointCount(aPointCount),
myType(eTYPE::DEBUGBOX),
mySpace(aSpace)
{
}

RenderCommand::RenderCommand(const Vector2f& aStartPosition, const Vector2f& anEndPosition, DWORD aColour, eSPACE aSpace) :
myStartPosition(aStartPosition),
myEndPosition(anEndPosition),
myColour(aColour),
myType(eTYPE::DEBUGLINE),
mySpace(aSpace)
{
}

RenderCommand::RenderCommand(const CU::Line<float>& aLine, DWORD aColour, eSPACE aSpace) :
myLine(aLine),
myColour(aColour),
myType(eTYPE::LINE),
mySpace(aSpace)
{
}

