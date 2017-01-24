#pragma once
#include "Math\Vector\Vector.h"
#include <Windows.h>
#include "Math\Line\Line.h"
#include <string>
class hgeSprite;
class hgeFont;

struct RenderCommand
{
	enum class eTYPE
	{
		SPRITE,
		TEXT,
		FORCEDTEXT,
		SPRITE_EX,
		DEBUGLINE,
		DEBUGBOX,
		DEBUGTRIANGLE,
		DEBUGCIRCLE,
		LINE,
	};

	enum class eSPACE
	{
		WORLD,
		SCREEN
	};

	//Sprite
	RenderCommand(const Vector2f& aPosition = { 0, 0 }, hgeSprite* aSprite = nullptr, eSPACE aSpace = eSPACE::WORLD);
	//Sprite_Ex (?)
	RenderCommand(const Vector2f& aPosition, const float aRot, Vector2f& aScale, hgeSprite* aSprite, eSPACE aSpace = eSPACE::WORLD);

	//Text
	RenderCommand(const Vector2f& aPosition, const char* aText, const int anAlignment = 0, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);
	RenderCommand(const Vector2f& aPosition, std::string aText, const int anAlignment = 0, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);
	RenderCommand(std::string aText, const int anAlignment = 0, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::WORLD);
	//Circle
	RenderCommand(const Vector2f& aPosition, const float aRadius, const int aSegmentAmount, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);
	//Box
	RenderCommand(const Vector2f& aPosition, const float aWidth, const float aHeight, int aPointCount = 4, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);
	//Lines
	RenderCommand(const Vector2f& aStartPosition, const Vector2f& anEndPosition, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);

	RenderCommand(const CU::Line<float>& aLine, DWORD aColour = 0xFFFFFFFF, eSPACE aSpace = eSPACE::SCREEN);


	Vector2f myPosition;
	Vector2f myStartPosition;
	Vector2f myEndPosition;
	hgeSprite* mySprite;
	float myRotation;
	Vector2f myScale;
	eTYPE myType;
	std::string myText;
	int myTextAlignment;
	DWORD myColour;
	eSPACE mySpace;
	float myRadius;
	int mySegmentAmount;
	int myPointCount;
	float myWidth;
	float myHeight;
	CU::Line<float> myLine;

};