#include "InputHandle.h"


#include <fstream>
#include <iostream>
#include <sstream>

#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#include "../Engine/Engine.h"

#include "hashlist.h"

#include "ControllerInput.h"
#include "InputWrapper.h"
#include "InputCommand.h"
void InputHandle::Initiate(u16 controller_ID)
{
	m_Controller = new ControllerInput(controller_ID);
	m_Input = new InputWrapper;
	Snowblind::CEngine* engine = Snowblind::CEngine::GetInstance();
	m_Input->Initiate(engine->GetWindow().GetHWND()
		, engine->GetWindow().GetWindowInstance());
}

void InputHandle::CleanUp()
{
	delete m_Controller;
	m_Controller = nullptr;

	delete m_Input;
	m_Input = nullptr;
}

void InputHandle::HandleInput()
{
	if (m_Controller->IsConnected())
	{
		if (m_Controller->ButtonDown(eXboxButton::X)) m_XButton();
		if (m_Controller->ButtonDown(eXboxButton::Y)) m_YButton();
		if (m_Controller->ButtonDown(eXboxButton::B)) m_BButton();
		if (m_Controller->ButtonDown(eXboxButton::A)) m_AButton();
	}

	if (m_Input->Update())
	{
		if (m_Input->KeyDown(KButton::SPACE))  m_SpaceBar();
		if (m_Input->KeyDown(KButton::W)) m_WKey();
		if (m_Input->KeyDown(KButton::S)) m_SKey();
		if (m_Input->KeyDown(KButton::A)) m_AKey();
		if (m_Input->KeyDown(KButton::D)) m_DKey();
	}
}

void InputHandle::Update()
{
	if (m_Controller->LeftThumbstickY() > 0.5f) m_LThumbYP();
	if (m_Controller->LeftThumbstickY() < -0.5f) m_LThumbYN();
	if (m_Controller->LeftThumbstickX() > 0.5f) m_LThumbXP();
	if (m_Controller->LeftThumbstickX() < -0.5f) m_LThumbXN();
}

void InputHandle::Bind(u32 hash, std::function<void()> function)
{
switch(hash)
{
	case s_EscKey_hash:
	{
		BindEscKey(function);
	}break;
	case s_TildeKey_hash:
	{
		BindTildeKey(function);
	}break;
	case s_1Key_hash:
	{
		Bind1Key(function);
	}break;
	case s_2Key_hash:
	{
		Bind2Key(function);
	}break;
	case s_3Key_hash:
	{
		Bind3Key(function);
	}break;
	case s_4Key_hash:
	{
		Bind4Key(function);
	}break;
	case s_5Key_hash:
	{
		Bind5Key(function);
	}break;
	case s_6Key_hash:
	{
		Bind6Key(function);
	}break;
	case s_7Key_hash:
	{
		Bind7Key(function);
	}break;
	case s_8Key_hash:
	{
		Bind8Key(function);
	}break;
	case s_9Key_hash:
	{
		Bind9Key(function);
	}break;
	case s_0Key_hash:
	{
		Bind0Key(function);
	}break;
	case s_DashKey_hash:
	{
		BindDashKey(function);
	}break;
	case s_EqualKey_hash:
	{
		BindEqualKey(function);
	}break;
	case s_Backspace_hash:
	{
		BindBackspace(function);
	}break;
	case s_TabKey_hash:
	{
		BindTabKey(function);
	}break;
	case s_QKey_hash:
	{
		BindQKey(function);
	}break;
	case s_EKey_hash:
	{
		BindEKey(function);
	}break;
	case s_WKey_hash:
	{
		BindWKey(function);
	}break;
	case s_RKey_hash:
	{
		BindRKey(function);
	}break;
	case s_TKey_hash:
	{
		BindTKey(function);
	}break;
	case s_YKey_hash:
	{
		BindYKey(function);
	}break;
	case s_UKey_hash:
	{
		BindUKey(function);
	}break;
	case s_IKey_hash:
	{
		BindIKey(function);
	}break;
	case s_OKey_hash:
	{
		BindOKey(function);
	}break;
	case s_PKey_hash:
	{
		BindPKey(function);
	}break;
	case s_LBracketKey_hash:
	{
		BindLBracketKey(function);
	}break;
	case s_RBracketKey_hash:
	{
		BindRBracketKey(function);
	}break;
	case s_ReturnKey_hash:
	{
		BindReturnKey(function);
	}break;
	case s_LCtrlKey_hash:
	{
		BindLCtrlKey(function);
	}break;
	case s_AKey_hash:
	{
		BindAKey(function);
	}break;
	case s_SKey_hash:
	{
		BindSKey(function);
	}break;
	case s_DKey_hash:
	{
		BindDKey(function);
	}break;
	case s_FKey_hash:
	{
		BindFKey(function);
	}break;
	case s_GKey_hash:
	{
		BindGKey(function);
	}break;
	case s_HKey_hash:
	{
		BindHKey(function);
	}break;
	case s_JKey_hash:
	{
		BindJKey(function);
	}break;
	case s_KKey_hash:
	{
		BindKKey(function);
	}break;
	case s_LKey_hash:
	{
		BindLKey(function);
	}break;
	case s_SemicolonKey_hash:
	{
		BindSemicolonKey(function);
	}break;
	case s_ApostropheKey_hash:
	{
		BindApostropheKey(function);
	}break;
	case s_GraveKey_hash:
	{
		BindGraveKey(function);
	}break;
	case s_LShiftKey_hash:
	{
		BindLShiftKey(function);
	}break;
	case s_BackslashKey_hash:
	{
		BindBackslashKey(function);
	}break;
	case s_ZKey_hash:
	{
		BindZKey(function);
	}break;
	case s_XKey_hash:
	{
		BindXKey(function);
	}break;
	case s_CKey_hash:
	{
		BindCKey(function);
	}break;
	case s_VKey_hash:
	{
		BindVKey(function);
	}break;
	case s_BKey_hash:
	{
		BindBKey(function);
	}break;
	case s_NKey_hash:
	{
		BindNKey(function);
	}break;
	case s_MKey_hash:
	{
		BindMKey(function);
	}break;
	case s_CommaKey_hash:
	{
		BindCommaKey(function);
	}break;
	case s_PeriodKey_hash:
	{
		BindPeriodKey(function);
	}break;
	case s_SlashKey_hash:
	{
		BindSlashKey(function);
	}break;
	case s_RShiftKey_hash:
	{
		BindRShiftKey(function);
	}break;
	case s_MultiplyKey_hash:
	{
		BindMultiplyKey(function);
	}break;
	case s_LAltKey_hash:
	{
		BindLAltKey(function);
	}break;
	case s_SpaceBar_hash:
	{
		BindSpaceBar(function);
	}break;
	case s_CapsLock_hash:
	{
		BindCapsLock(function);
	}break;
	case s_F1Key_hash:
	{
		BindF1Key(function);
	}break;
	case s_F2Key_hash:
	{
		BindF2Key(function);
	}break;
	case s_F3Key_hash:
	{
		BindF3Key(function);
	}break;
	case s_F4Key_hash:
	{
		BindF4Key(function);
	}break;
	case s_F5Key_hash:
	{
		BindF5Key(function);
	}break;
	case s_F6Key_hash:
	{
		BindF6Key(function);
	}break;
	case s_F7Key_hash:
	{
		BindF7Key(function);
	}break;
	case s_F8Key_hash:
	{
		BindF8Key(function);
	}break;
	case s_F9Key_hash:
	{
		BindF9Key(function);
	}break;
	case s_F10Key_hash:
	{
		BindF10Key(function);
	}break;
	case s_F11Key_hash:
	{
		BindF11Key(function);
	}break;
	case s_F12Key_hash:
	{
		BindF12Key(function);
	}break;
	case s_F13Key_hash:
	{
		BindF13Key(function);
	}break;
	case s_F14Key_hash:
	{
		BindF14Key(function);
	}break;
	case s_F15Key_hash:
	{
		BindF15Key(function);
	}break;
	case s_NumLock_hash:
	{
		BindNumLock(function);
	}break;
	case s_Numpad1_hash:
	{
		BindNumpad1(function);
	}break;
	case s_Numpad2_hash:
	{
		BindNumpad2(function);
	}break;
	case s_Numpad3_hash:
	{
		BindNumpad3(function);
	}break;
	case s_Numpad4_hash:
	{
		BindNumpad4(function);
	}break;
	case s_Numpad5_hash:
	{
		BindNumpad5(function);
	}break;
	case s_Numpad6_hash:
	{
		BindNumpad6(function);
	}break;
	case s_Numpad7_hash:
	{
		BindNumpad7(function);
	}break;
	case s_Numpad8_hash:
	{
		BindNumpad8(function);
	}break;
	case s_Numpad9_hash:
	{
		BindNumpad9(function);
	}break;
	case s_Numpad0_hash:
	{
		BindNumpad0(function);
	}break;
	case s_NumpadAdd_hash:
	{
		BindNumpadAdd(function);
	}break;
	case s_NumpadMinus_hash:
	{
		BindNumpadMinus(function);
	}break;
	case s_NumpadMultiply_hash:
	{
		BindNumpadMultiply(function);
	}break;
	case s_NumpadDecimal_hash:
	{
		BindNumpadDecimal(function);
	}break;
	case s_NumpadDivide_hash:
	{
		BindNumpadDivide(function);
	}break;
	case s_ArrowUp_hash:
	{
		BindArrowUp(function);
	}break;
	case s_ArrowDown_hash:
	{
		BindArrowDown(function);
	}break;
	case s_ArrowLeft_hash:
	{
		BindArrowLeft(function);
	}break;
	case s_ArrowRight_hash:
	{
		BindArrowRight(function);
	}break;
	case s_KeyDelete_hash:
	{
		BindKeyDelete(function);
	}break;
	case s_InsertKey_hash:
	{
		BindInsertKey(function);
	}break;
	case s_PageDown_hash:
	{
		BindPageDown(function);
	}break;
	case s_PageUp_hash:
	{
		BindPageUp(function);
	}break;
	case s_HomeKey_hash:
	{
		BindHomeKey(function);
	}break;
	case s_EndKey_hash:
	{
		BindEndKey(function);
	}break;
	case s_ScrollLock_hash:
	{
		BindScrollLock(function);
	}break;
	case s_PrintScreen_hash:
	{
		BindPrintScreen(function);
	}break;
	case s_PauseBreak_hash:
	{
		BindPauseBreak(function);
	}break;
	case s_XButton_hash:
	{
		BindXButton(function);
	}break;
	case s_YButton_hash:
	{
		BindYButton(function);
	}break;
	case s_BButton_hash:
	{
		BindBButton(function);
	}break;
	case s_AButton_hash:
	{
		BindAButton(function);
	}break;
	case s_LThumbYP_hash:
	{
		BindLThumbYP(function);
	}break;
	case s_LThumbYN_hash:
	{
		BindLThumbYN(function);
	}break;
	case s_LThumbXP_hash:
	{
		BindLThumbXP(function);
	}break;
	case s_LThumbXN_hash:
	{
		BindLThumbXN(function);
	}break;
	case s_RThumbYP_hash:
	{
		BindRThumbYP(function);
	}break;
	case s_RThumbYN_hash:
	{
		BindRThumbYN(function);
	}break;
	case s_RThumbXP_hash:
	{
		BindRThumbXP(function);
	}break;
	case s_RThumbXN_hash:
	{
		BindRThumbXN(function);
	}break;
	case s_RTrigger_hash:
	{
		BindRTrigger(function);
	}break;
	case s_RBumper_hash:
	{
		BindRBumper(function);
	}break;
	case s_LTrigger_hash:
	{
		BindLTrigger(function);
	}break;
	case s_LBumper_hash:
	{
		BindLBumper(function);
	}break;
	case s_DPadUp_hash:
	{
		BindDPadUp(function);
	}break;
	case s_DPadDown_hash:
	{
		BindDPadDown(function);
	}break;
	case s_DPadRight_hash:
	{
		BindDPadRight(function);
	}break;
	case s_DPadLeft_hash:
	{
		BindDPadLeft(function);
	}break;
	}
}
