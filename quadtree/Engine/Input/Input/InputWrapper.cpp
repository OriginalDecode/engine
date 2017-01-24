#include "InputWrapper.h"
#define RANGE(VAR, MIN, MAX) ((VAR) < (MIN)) ? (MIN) : ((VAR) > (MAX)) ? (MAX) : (VAR)
namespace CommonUtilities
{
	namespace Input
	{

		InputWrapper::InputWrapper()
		{
			RECT desktop;
			const HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);
			myDimensions.x = static_cast<float>(desktop.right);
			myDimensions.y = static_cast<float>(desktop.bottom);
			myOldMousePos = 0;
			myMousePos = 0;
		}

		void InputWrapper::Initiate(HWND ahWnd)
		{
			DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&myDInput, nullptr);
			myDInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, nullptr);
			myDInput->CreateDevice(GUID_SysMouse, &myMouse, nullptr);

			myKeyboard->SetDataFormat(&c_dfDIKeyboard);
			myMouse->SetDataFormat(&c_dfDIMouse);

			myKeyboard->SetCooperativeLevel(ahWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			myMouse->SetCooperativeLevel(ahWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

			myKeyboard->Acquire();
			myMouse->Acquire();

			SetMouseSpeed(1);
			SetCursorPosition({ 10, 10 });

			myCursorIsLocked = true;
			myHWND = ahWnd;
			DL_DEBUG("Input Wrapper Initiated");
		}

		void InputWrapper::Update()
		{
			if (GetActiveWindow() != false)
			{

				memcpy_s(&myPrevKeyState, sizeof(myPrevKeyState), myKeyState, sizeof(myKeyState));
				memcpy_s(&myPrevMouseState, sizeof(myPrevMouseState), &myMouseState, sizeof(myMouseState));

				HRESULT hr = myKeyboard->GetDeviceState(sizeof(myKeyState), (VOID**)&myKeyState);
				if (FAILED(hr))
				{
					ZeroMemory(myKeyState, sizeof(myKeyState));

					myKeyboard->Acquire();
				}

				hr = myMouse->GetDeviceState(sizeof(DIMOUSESTATE), (VOID**)&myMouseState);
				if (FAILED(hr))
				{
					ZeroMemory(&myMouseState, sizeof(myMouseState));

					myMouse->Acquire();
				}
				
				GetPhysicalCursorPos(&myCursorPos);
				myCoord.x = (float)myCursorPos.x;
				myCoord.y = (float)myCursorPos.y;
				
				//tagPOINT cursorXandY;
				GetCursorPos(&myCursorPos);
				ScreenToClient(myHWND, &myCursorPos);
				myMousePos.x = static_cast<float>(myCursorPos.x);
				myMousePos.y = static_cast<float>(myCursorPos.y);

				/*UpdateMousePosition();
				if (myCursorIsLocked == true)
					KeepInside();*/

			}
		}

		float InputWrapper::MouseDirectX()
		{
			return static_cast<float>(myMouseState.lX);
		}

		float InputWrapper::MouseDirectY()
		{
			return static_cast<float>(myMouseState.lY);
		}

		bool InputWrapper::MouseClick(int aButton)
		{
			return (myMouseState.rgbButtons[aButton] & 0x80) != 0 && (myPrevMouseState.rgbButtons[aButton] & 0x80) == 0;
		}

		bool InputWrapper::MouseDown(int aButton)
		{
			return (myMouseState.rgbButtons[aButton] & 0x80) != 0;
		}

		bool InputWrapper::MouseUp(int aButton)
		{
			return (myMouseState.rgbButtons[aButton] & 0x80) == 0 && (myPrevMouseState.rgbButtons[aButton] & 0x80) != 0;
		}

		bool InputWrapper::KeyDown(KButton aKey)
		{
			return (myKeyState[UCHAR(aKey)] & 0x80) != 0;
		}

		bool InputWrapper::KeyClick(KButton aKey)
		{
			return (myKeyState[UCHAR(aKey)] & 0x80) != 0 && (myPrevKeyState[UCHAR(aKey)] & 0x80) == 0;
		}

		bool InputWrapper::KeyUp(KButton aKey)
		{
			return (myKeyState[UCHAR(aKey)] & 0x80) == 0 && (myPrevKeyState[UCHAR(aKey)] & 0x80) != 0;
		}

		bool InputWrapper::KeyDown(UCHAR aKey)
		{
			return (myKeyState[aKey] & 0x80) != 0;
		}

		bool InputWrapper::KeyClick(UCHAR aKey)
		{
			return (myKeyState[aKey] & 0x80) != 0 && (myPrevKeyState[aKey] & 0x80) == 0;
		}

		bool InputWrapper::KeyUp(UCHAR aKey)
		{
			return (myKeyState[aKey] & 0x80) == 0 && (myPrevKeyState[aKey] & 0x80) != 0;
		}

		const Vector2f& InputWrapper::GetMousePos() const
		{
			return myMousePos;
		}

		void InputWrapper::UpdateMousePosition()
		{
			myOldMousePos = myMousePos;

			myMousePos.x += float(myMouseState.lX * myMouseSpeed);
			myMousePos.y += float(myMouseState.lY * myMouseSpeed);

			myMousePos.x = RANGE(myMousePos.x, 1, myScreen.x);
			myMousePos.y = RANGE(myMousePos.y, 1, myScreen.y);
		}

		Vector2f InputWrapper::GetMouseDeltaPositionInPixels() const
		{
			Vector2f position;

			position.x = myMousePos.x - myOldMousePos.x;
			position.y = myMousePos.y - myOldMousePos.y;

			return position;
		}

		void InputWrapper::SetMouseSpeed(const float aMouseSpeed)
		{
			myMouseSpeed = aMouseSpeed;
			if (myMouseSpeed < 0.01f)
				myMouseSpeed = 0.01f;
		}

		void InputWrapper::CaptureWindowsCursor()
		{
			if (myCursorIsLocked == true)
				myCursorIsLocked = false;
			else if (myCursorIsLocked == false)
				myCursorIsLocked = true;
		}

		void InputWrapper::ReleaseCursor()
		{
			myCursorIsLocked = false;
		}


		void InputWrapper::KeepInside()
		{

			RECT rect;
			GetWindowRect(myHWND, &rect);
			SetCursorPos(int(rect.left + (myScreen.x / 2)),int(rect.top + (myScreen.y / 2)));

		}

		void InputWrapper::SetCursorPosition(Vector2f aPosition)
		{
			myMousePos.x = aPosition.x;
			myMousePos.y = aPosition.y;
		}
	}
}