#include "InputWrapper.h"
#include <assert.h>

#define RANGE(VAR, MIN, MAX) ((VAR) < (MIN)) ? (MIN) : ((VAR) > (MAX)) ? (MAX) : (VAR)
namespace CommonUtilities
{
	namespace Input
	{

		InputWrapper* InputWrapper::myInstance = nullptr;

		InputWrapper::InputWrapper(HWND aHWND, HINSTANCE hInstance)
		{
			RECT desktop;
			const HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);
			Initiate(aHWND, hInstance);
		}

		InputWrapper::~InputWrapper()
		{

		}

		void InputWrapper::Create(HWND aHWND, HINSTANCE hInstance)
		{
			if (myInstance == nullptr)
			{
				myInstance = new InputWrapper(aHWND, hInstance);
			}
		}

		void InputWrapper::Destroy()
		{
			if (myInstance != nullptr)
			{
				delete myInstance;
				myInstance = nullptr;
			}
		}

		InputWrapper* InputWrapper::GetInstance()
		{
			assert(myInstance != nullptr && "Input wrapper were null!");
			return myInstance;
		}

		void InputWrapper::Initiate(HWND aHWND, HINSTANCE hInstance)
		{
			DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&myDInput, nullptr);
			myDInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, nullptr);
			myDInput->CreateDevice(GUID_SysMouse, &myMouse, nullptr);

			myKeyboard->SetDataFormat(&c_dfDIKeyboard);
			myMouse->SetDataFormat(&c_dfDIMouse);

			myKeyboard->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			myMouse->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

			myKeyboard->Acquire();
			myMouse->Acquire();

			SetMouseSpeed(1);

			LockCursor(false);
			myHWND = aHWND;
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
				ScreenToClient(myHWND, &myCursorPos);
				myMousePos.myX = float(myCursorPos.x);
				myMousePos.myY = float(myCursorPos.y);

				myMousePos.myZ += float(myMouseState.lZ);
				if (myCursorIsLocked == true)
				{
					SetCursorPos(0, 0);
				}
				//tagPOINT cursorXandY;
				GetCursorPos(&myCursorPos);
				ScreenToClient(myHWND, &myCursorPos);

			}
		}

		void InputWrapper::LockCursor(bool someLock)
		{
			myCursorIsLocked = someLock;
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

		void InputWrapper::SetMouseSpeed(const float aMouseSpeed)
		{
			myMouseSpeed = aMouseSpeed;
			if (myMouseSpeed < 0.01f)
				myMouseSpeed = 0.01f;
		}

		void InputWrapper::CaptureWindowsCursor()
		{
			myCursorIsLocked = !myCursorIsLocked;
		}

		void InputWrapper::ReleaseCursor()
		{
			myCursorIsLocked = false;
		}
	}
}