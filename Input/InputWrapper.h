#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>
#include <standard_datatype.hpp>
#include "../CommonLib/Math/Vector/Vector.h"

enum class MouseInput
{
	LEFT,
	RIGHT,
	SCROLL,
	SIDE_UP,
	SIDE_DOWN
};

enum class KButton
{

	A = DIK_A,
	B = DIK_B,
	C = DIK_C,
	D = DIK_D,
	E = DIK_E,
	F = DIK_F,
	G = DIK_G,
	H = DIK_H,
	I = DIK_I,
	J = DIK_J,
	K = DIK_K,
	L = DIK_L,
	M = DIK_M,
	N = DIK_N,
	O = DIK_O,
	P = DIK_P,
	Q = DIK_Q,
	R = DIK_R,
	S = DIK_S,
	T = DIK_T,
	U = DIK_U,
	V = DIK_V,
	W = DIK_W,
	X = DIK_X,
	Y = DIK_Y,
	Z = DIK_Z,
	NUM0 = DIK_0,
	NUM1 = DIK_1,
	NUM2 = DIK_2,
	NUM3 = DIK_3,
	NUM4 = DIK_4,
	NUM5 = DIK_5,
	NUM6 = DIK_6,
	NUM7 = DIK_7,
	NUM8 = DIK_8,
	NUM9 = DIK_9,
	NUMPAD0 = DIK_NUMPAD0,
	NUMPAD1 = DIK_NUMPAD1,
	NUMPAD2 = DIK_NUMPAD2,
	NUMPAD3 = DIK_NUMPAD3,
	NUMPAD4 = DIK_NUMPAD4,
	NUMPAD5 = DIK_NUMPAD5,
	NUMPAD6 = DIK_NUMPAD6,
	NUMPAD7 = DIK_NUMPAD7,
	NUMPAD8 = DIK_NUMPAD8,
	NUMPAD9 = DIK_NUMPAD9,
	TAB = DIK_TAB,
	ENTER = DIK_RETURN,
	UP_ARROW = DIK_UPARROW,
	DOWN_ARROW = DIK_DOWNARROW,
	RIGHT_ARROW = DIK_RIGHTARROW,
	LEFT_ARROW = DIK_LEFTARROW,
	BACKSPACE = DIK_BACKSPACE,
	SPACE = DIK_SPACE,
	LCTRL = DIK_LCONTROL,
	RCTRL = DIK_RCONTROL,
	LSHIFT = DIK_LSHIFT,
	RSHIFT = DIK_RSHIFT,
	LALT = DIK_LALT,
	RALT = DIK_RALT,
	NENTER = DIK_NUMPADENTER,
	F1 = DIK_F1,
	F2 = DIK_F2,
	F3 = DIK_F3,
	F4 = DIK_F4,
	F5 = DIK_F5,
	F6 = DIK_F6,
	F7 = DIK_F7,
	F8 = DIK_F8,
	F9 = DIK_F9,
	F10 = DIK_F10,
	F11 = DIK_F11,
	F12 = DIK_F12,
	ESCAPE = DIK_ESCAPE,
	KDELETE = DIK_DELETE,
	INSERT = DIK_INSERT,
	PGDOWN = DIK_PGDN,
	PGUP = DIK_PGUP,
	HOME = DIK_HOME,
	END = DIK_END,
	NUMADD = DIK_ADD,
	NUMMINUS = DIK_SUBTRACT
};


class InputWrapper
{
public:
	InputWrapper() = default;
	
	bool Initiate(HWND aHWND, HINSTANCE hInstance);
	bool CleanUp();
	void Update();


	//________________________
	// KEYBOARD
	bool IsDown(KButton aKey);
	bool OnDown(KButton aKey);
	bool OnRelease(KButton aKey);

	bool IsDown(UCHAR aKey);
	bool OnDown(UCHAR aKey);
	bool OnRelease(UCHAR aKey);

	bool PS4IsDown(UCHAR button);
	bool PS4OnDown(UCHAR button);
	bool PS4OnRelease(UCHAR button);


	//________________________
	// MOUSE
	bool OnClick(MouseInput button);
	bool IsDown(MouseInput button);
	bool OnRelease(MouseInput button);

	CU::Vector2f GetCursorPos() const { return m_CurorPos; }

	float MouseDirectX();
	float MouseDirectY();


private:
	HWND myHWND;
	IDirectInput8* myDInput;

	/* Keyboard */
	IDirectInputDevice8* myKeyboard;
	UCHAR myKeyState[256];
	UCHAR myPrevKeyState[256];
	
	/* Mouse */
	IDirectInputDevice8* myMouse;
	DIMOUSESTATE2 myMouseState;
	DIMOUSESTATE2 myPrevMouseState;

	IDirectInputDevice8* m_PS4;
	UCHAR m_ButtonState[256];
	UCHAR m_PrevButtonState[256];

	POINT myCursorPos;
	CU::Vector2f m_CurorPos;
};
