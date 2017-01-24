#include "Renderer.h"
#include "Misc\Megaton.h"
#include <sstream>
#include "Synchronizer.h"
Renderer::Renderer(HGE& aHGE, Synchronizer& aSychronizer) :
myHGE(aHGE),
mySynchronizer(aSychronizer),
myFont("../Data/Assets/Fonts/font_1.fnt")
{
	myMultiplier = 0;
	myTextBuffer.Init(128);
	myForcedTextBuffer.Init(128);

}

Renderer::Renderer(const Renderer& aRenderer) :
myHGE(aRenderer.myHGE),
mySynchronizer(aRenderer.mySynchronizer),
myFont("../Data/Assets/Fonts/font_1.fnt")
{
	myMultiplier = aRenderer.myMultiplier;
	myResolution = aRenderer.myResolution;
	myTextBuffer.Init(128);
	myForcedTextBuffer.Init(128);

}

Renderer::~Renderer()
{

}

void Renderer::Initiate()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	myResolution.x = float(desktop.right);
	myResolution.y = float(desktop.bottom);
}


void Renderer::Render()
{

	while (mySynchronizer.HasQuit() == false)
	{
		myHGE.Gfx_BeginScene();
		myHGE.Gfx_Clear(/*0xFF50517D*/0);

		const CU::GrowingArray<RenderCommand>& renderCommands = mySynchronizer.GetRenderCommands();

		for (int i = 0; i < renderCommands.Size(); ++i)
		{
			const RenderCommand& renderCommand = renderCommands[i];
			Vector2f pos = renderCommand.myPosition;
			//	float resolutionOffset = 1.f;

			if (renderCommand.mySpace == RenderCommand::eSPACE::WORLD)
			{
				pos *= mySynchronizer.GetCamera();
				pos += 1.f;
				pos *= 0.5f;
				pos *= myResolution;
			}
			pos;
			pos.x = rint(pos.x);
			pos.y = rint(pos.y);

			Vector2f res = myResolution;

			switch (renderCommand.myType)
			{
			case RenderCommand::eTYPE::SPRITE:
				renderCommand.mySprite->Render(pos.x, pos.y);
				break;
			case RenderCommand::eTYPE::SPRITE_EX:
				renderCommand.mySprite->RenderEx(pos.x, pos.y, renderCommand.myRotation, renderCommand.myScale.x, renderCommand.myScale.y);
				break;
			case RenderCommand::eTYPE::TEXT:
				myTextBuffer.Add(renderCommand);
				break;
			case RenderCommand::eTYPE::FORCEDTEXT:
				myForcedTextBuffer.Add(renderCommand);
				break;
			case RenderCommand::eTYPE::DEBUGBOX:
				DrawBox(renderCommand.myPosition, renderCommand.myWidth, renderCommand.myHeight, renderCommand.myPointCount, renderCommand.myColour);
				break;
			case RenderCommand::eTYPE::DEBUGLINE:
				DrawLine(renderCommand.myStartPosition, renderCommand.myEndPosition, renderCommand.myColour);
				break;
			case RenderCommand::eTYPE::DEBUGCIRCLE:
				DrawCircle(renderCommand.myPosition, renderCommand.myRadius, renderCommand.mySegmentAmount, renderCommand.myColour);
				break;
			case RenderCommand::eTYPE::LINE:
				break;
			default:
				break;
			}
		}

		//Not forced
		for (int i = 0; i < myTextBuffer.Size(); ++i)
		{
			const RenderCommand& renderCommand = myTextBuffer[i];
			Vector2f pos = renderCommand.myPosition;
			myFont.SetColor(renderCommand.myColour);
			myFont.Render(pos.x, pos.y, renderCommand.myTextAlignment, renderCommand.myText.c_str());
		}
		myTextBuffer.RemoveAll();

		//Forced
		std::stringstream ss;
		std::stringstream activateDebug;
		float textYPos = 0.f;
		if (GET_DEBUG == false)
		{
			activateDebug << "Show Debug Info [F2]";
		}
		else if (GET_DEBUG == true)
		{
			activateDebug << "Hide Debug Info [F2]";
			ss << "Render Calls : ";
			ss << renderCommands.Size();

		}
		myForcedTextBuffer.Add(RenderCommand(ss.str().c_str(), 0, 0xFFFFFFFF, RenderCommand::eSPACE::SCREEN));
		myForcedTextBuffer.Add(RenderCommand(activateDebug.str().c_str(), 0, 0xFFFFFFFF, RenderCommand::eSPACE::SCREEN));





		for (int i = 0; i < myForcedTextBuffer.Size(); ++i)
		{
			textYPos = i * 20.f;
			if (GET_DEBUG == false)
			{
				textYPos -= 20.f;
			}
			const RenderCommand& renderCommand = myForcedTextBuffer[i];
			myFont.SetColor(renderCommand.myColour);
			myFont.Render(5.f, textYPos, renderCommand.myTextAlignment, renderCommand.myText.c_str());
		}
		myForcedTextBuffer.RemoveAll();

		myHGE.Gfx_EndScene();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();

	}
}

void Renderer::DrawLine(Vector2f aStart, Vector2f aEnd, DWORD aColour)
{

	myHGE.Gfx_RenderLine(aStart.x, aStart.y, aEnd.x, aEnd.y, aColour);

}

void Renderer::DrawLine(CU::Line<float>& aLine, DWORD aColour)
{
	DrawLine(aLine.myStart, { aLine.myData.x, aLine.myData.y }, aColour);
}

void Renderer::DrawBox(Vector2f anOriginPosition, float aWidth, float aHeight, int aPointCount, DWORD aColour)
{
	if (aPointCount == 4)
	{
		Vector2f position2 = { anOriginPosition.x + aWidth, anOriginPosition.y };
		Vector2f position3 = { position2.x, position2.y + aHeight };
		Vector2f position4 = { anOriginPosition.x, anOriginPosition.y + aHeight };

		DrawLine(anOriginPosition, position2, aColour);
		DrawLine(position2, position3, aColour);
		DrawLine(position3, position4, aColour);
		DrawLine(position4, anOriginPosition, aColour);
	}
	else if (aPointCount == 3)
	{
		DrawTriangle(anOriginPosition, aHeight, aWidth, aColour);
	}
	else
		return;
}

void Renderer::DrawCircle(Vector2f anOriginPosition, float aRadius, int aSegmentAmount, DWORD aColour)
{
	float angle;

	Vector2f first;
	Vector2f second;
	angle = 2.f * float(M_PI) / float(aSegmentAmount);

	second.x = aRadius;
	second.y = 0.f;

	for (float a = 0; a <= (2.f*M_PI + angle); a += angle)
	{
		first.x = second.x;
		first.y = second.y;

		second.x = aRadius * cos(a);
		second.y = aRadius * sin(a);

		Vector2f start(first.x + anOriginPosition.x + aRadius, first.y + anOriginPosition.y + aRadius);
		Vector2f end(second.x + anOriginPosition.x + aRadius, second.y + anOriginPosition.y + aRadius);

		DrawLine(start, end, aColour);
	}
}

void Renderer::DrawTriangle(Vector2f aFirstPosition, float aDistance, float aWidth, DWORD aColour)
{

	Vector2f position2(aFirstPosition.x - (aWidth / 2.f), aFirstPosition.y + aDistance);
	Vector2f position3(aFirstPosition.x + (aWidth / 2.f), aFirstPosition.y + aDistance);

	DrawLine(aFirstPosition, position2, aColour);
	DrawLine(aFirstPosition, position3, aColour);
	DrawLine(position2, position3, aColour);

	//DrawLine(position4, aFirstPosition, aColour);

}