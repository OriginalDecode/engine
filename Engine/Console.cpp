#include "stdafx.h"
#include "Console.h"
#include "Sprite.h"
#include "TextureContainer.h"
#include "Engine.h"
#include "InputWrapper.h"
#include "EngineDefines.h"
namespace Snowblind
{
	CConsole::CConsole()
	{

	}


	CConsole::~CConsole()
	{
		SAFE_DELETE(myText);
		SAFE_DELETE(myInputText);
		SAFE_DELETE(mySprite);
		SAFE_DELETE(myTimeManager);
	}

	void CConsole::Initiate(CCamera* aCamera)
	{
		myCamera = aCamera;
		myIsActive = false;

		myInputWrapper = CU::Input::InputWrapper::GetInstance();

		myBottomLeftPosition = { (Snowblind::CEngine::GetInstance()->GetWindowSize().myWidth / 2.f) + 4.f, (Snowblind::CEngine::GetInstance()->GetWindowSize().myHeight / 2.f) - 24.f };
		myTopLeftPosition = { (Snowblind::CEngine::GetInstance()->GetWindowSize().myWidth / 2.f) + 4.f, 0.f };

		mySprite = new CSprite();
		mySprite->Initiate("Data/Textures/consoleBG.dds", { Snowblind::CEngine::GetInstance()->GetWindowSize().myWidth / 2.f, 
															Snowblind::CEngine::GetInstance()->GetWindowSize().myHeight / 2.f },
															{ 0, 0 });
		mySprite->SetHotspot({ -Snowblind::CEngine::GetInstance()->GetWindowSize().myWidth / 4.f, Snowblind::CEngine::GetInstance()->GetWindowSize().myHeight / 4.f });
		mySprite->SetPosition({ CEngine::GetInstance()->GetWindowSize().myWidth, 0 });

		myText = new CText("Data/Font/OpenSans-Bold.ttf", 16, myCamera);
		myText->SetPosition(myTopLeftPosition);

		myInputText = new CText("Data/Font/OpenSans-Bold.ttf", 16, myCamera);
		myInputText->SetPosition(myBottomLeftPosition);
		myTimeManager = new CU::TimeManager();
		myDownTime = 0.f;
	}

	void CConsole::Render()
	{
		if (myIsActive)
		{
			CEngine::GetDirectX()->DisableZBuffer();
			mySprite->Render(myCamera);

			for (int i = 0; i < myStrings.Size(); i++)
			{
				myText->SetText(myStrings[i]);
				myText->SetPosition({ myTopLeftPosition.x,myTopLeftPosition.y + (i * 18) });
				myText->Render();
			}
			myInputText->Render();
			CEngine::GetDirectX()->EnableZBuffer();
		}
	}

	void CConsole::Update()
	{
		if (myIsActive)
		{
			myTimeManager->Update();
			myDeltaTime = myTimeManager->GetDeltaTime();
			ReadInput();
			if (myInput != "")
				myInputText->SetText(myInput);
		}
	}

	void CConsole::ToggleConsole()
	{
		myIsActive = !myIsActive;
	}

	void CConsole::PrintToConsole(const std::string& aMessage)
	{
		myStrings.Add(aMessage);
	}

	void CConsole::ReadInput()
	{
		bool shift = myInputWrapper->KeyDown(DIK_LSHIFT) || myInputWrapper->KeyDown(DIK_RSHIFT);
		bool mark = myInputWrapper->KeyDown(DIK_LCONTROL) && myInputWrapper->KeyDown(A);
		bool cpy = myInputWrapper->KeyDown(DIK_LCONTROL) && myInputWrapper->KeyDown(C);
		bool paste = myInputWrapper->KeyDown(DIK_LCONTROL) && myInputWrapper->KeyClick(V);

		if (mark)
		{
			myMarkedText = myInput;
		}

		if (myMarkedText != "" && cpy)
		{
			myCopiedText = myMarkedText;
			cpy = false;
			mark = false;
		}

		if (myMarkedText != "" && paste)
		{
			myInput += myCopiedText;
			paste = false;
			mark = false;
		}

		if (myMarkedText != "" && (myInputWrapper->KeyClick(DIK_DELETE) || myInputWrapper->KeyClick(DIK_BACKSPACE)))
		{
			myInput = "";
			myMarkedText = "";
			myInputText->SetText(" ");
			mark = false;
		}



		if (myInputWrapper->KeyDown(DIK_BACKSPACE))
		{

			myDownTime += myDeltaTime;
			if (myDownTime > 0.3f)
			{
				myEraseTime -= myDeltaTime;
				if (myEraseTime < 0.f)
				{
					if (myInput != "")
					{
						myInput.erase(myInput.end() - 1, myInput.end());
					}
					myEraseTime = 0.01f;
				}

			}
			if (myInput.size() <= 0)
			{
				myInputText->SetText(" ");
			}
		}

		if (myInputWrapper->KeyUp(DIK_BACKSPACE))
		{
			myDownTime = 0.f;
		}

		if (myInputWrapper->KeyClick(DIK_BACKSPACE))
		{
			if (myInput != "")
			{
				myInput.erase(myInput.end() - 1, myInput.end());
			}
			if (myInput.size() <= 0)
			{
				myInputText->SetText(" ");
			}
		}

		if (!myInputWrapper->KeyDown(DIK_LCONTROL))
		{

			myInput += myInputWrapper->KeyClick(DIK_A) ? shift ? "A" : "a" : "";
			myInput += myInputWrapper->KeyClick(DIK_B) ? shift ? "B" : "b" : "";
			myInput += myInputWrapper->KeyClick(DIK_C) ? shift ? "C" : "c" : "";
			myInput += myInputWrapper->KeyClick(DIK_D) ? shift ? "D" : "d" : "";
			myInput += myInputWrapper->KeyClick(DIK_E) ? shift ? "E" : "e" : "";
			myInput += myInputWrapper->KeyClick(DIK_F) ? shift ? "F" : "f" : "";
			myInput += myInputWrapper->KeyClick(DIK_G) ? shift ? "G" : "g" : "";
			myInput += myInputWrapper->KeyClick(DIK_H) ? shift ? "H" : "h" : "";
			myInput += myInputWrapper->KeyClick(DIK_I) ? shift ? "I" : "i" : "";
			myInput += myInputWrapper->KeyClick(DIK_J) ? shift ? "J" : "j" : "";
			myInput += myInputWrapper->KeyClick(DIK_K) ? shift ? "K" : "k" : "";
			myInput += myInputWrapper->KeyClick(DIK_L) ? shift ? "L" : "l" : "";
			myInput += myInputWrapper->KeyClick(DIK_M) ? shift ? "M" : "m" : "";
			myInput += myInputWrapper->KeyClick(DIK_N) ? shift ? "N" : "n" : "";
			myInput += myInputWrapper->KeyClick(DIK_O) ? shift ? "O" : "o" : "";
			myInput += myInputWrapper->KeyClick(DIK_P) ? shift ? "P" : "p" : "";
			myInput += myInputWrapper->KeyClick(DIK_Q) ? shift ? "Q" : "q" : "";
			myInput += myInputWrapper->KeyClick(DIK_R) ? shift ? "R" : "r" : "";
			myInput += myInputWrapper->KeyClick(DIK_S) ? shift ? "S" : "s" : "";
			myInput += myInputWrapper->KeyClick(DIK_T) ? shift ? "T" : "t" : "";
			myInput += myInputWrapper->KeyClick(DIK_U) ? shift ? "U" : "u" : "";
			myInput += myInputWrapper->KeyClick(DIK_V) ? shift ? "V" : "v" : "";
			myInput += myInputWrapper->KeyClick(DIK_W) ? shift ? "W" : "w" : "";
			myInput += myInputWrapper->KeyClick(DIK_X) ? shift ? "X" : "x" : "";
			myInput += myInputWrapper->KeyClick(DIK_Y) ? shift ? "Y" : "y" : "";
			myInput += myInputWrapper->KeyClick(DIK_Z) ? shift ? "Z" : "z" : "";

			myInput += myInputWrapper->KeyClick(DIK_0) ? shift ? "=" : "0" : "";
			myInput += myInputWrapper->KeyClick(DIK_1) ? shift ? "!" : "1" : "";
			myInput += myInputWrapper->KeyClick(DIK_2) ? shift ? "\"" : "2" : "";
			myInput += myInputWrapper->KeyClick(DIK_3) ? shift ? "#" : "3" : "";
			myInput += myInputWrapper->KeyClick(DIK_4) ? shift ? "¤" : "4" : "";
			myInput += myInputWrapper->KeyClick(DIK_5) ? shift ? "%" : "5" : "";
			myInput += myInputWrapper->KeyClick(DIK_6) ? shift ? "&" : "6" : "";
			myInput += myInputWrapper->KeyClick(DIK_7) ? shift ? "/" : "7" : "";
			myInput += myInputWrapper->KeyClick(DIK_8) ? shift ? "(" : "8" : "";
			myInput += myInputWrapper->KeyClick(DIK_9) ? shift ? ")" : "9" : "";

			myInput += myInputWrapper->KeyClick(DIK_NUMPAD0) ? shift ? "0" : "0" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD1) ? shift ? "1" : "1" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD2) ? shift ? "2" : "2" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD3) ? shift ? "3" : "3" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD4) ? shift ? "4" : "4" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD5) ? shift ? "5" : "5" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD6) ? shift ? "6" : "6" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD7) ? shift ? "7" : "7" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD8) ? shift ? "8" : "8" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPAD9) ? shift ? "9" : "9" : "";

			myInput += myInputWrapper->KeyClick(DIK_NUMPADPLUS) ? shift ? "+" : "+" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPADMINUS) ? shift ? "-" : "-" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPADSTAR) ? shift ? "*" : "*" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPADSLASH) ? shift ? "/" : "/" : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPADCOMMA) ? shift ? "," : "," : "";
			myInput += myInputWrapper->KeyClick(DIK_NUMPADPERIOD) ? shift ? "." : "." : "";

			myInput += myInputWrapper->KeyClick(DIK_COMMA) ? shift ? ";" : "," : "";
			myInput += myInputWrapper->KeyClick(DIK_PERIOD) ? shift ? ":" : "." : "";
			myInput += myInputWrapper->KeyClick(53) ? shift ? "_" : "-" : "";

			if (myInputWrapper->KeyClick(DIK_SPACE))
			{
				myInput += " ";
			}

			if (myInput != "")
			{
				if (myInputWrapper->KeyClick(DIK_RETURN) || myInputWrapper->KeyClick(DIK_NUMPADENTER))
				{
					myStrings.Add(myInput);

					if (myInput == "Clear")
					{
						myStrings.RemoveAll();
					}

					myInput = "";
					myInputText->SetText(" ");
				}
			}
		}
	}

};