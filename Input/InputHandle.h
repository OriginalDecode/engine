#pragma once
#include <standard_datatype.hpp>
#include <functional>
class ControllerInput;
class InputWrapper;
#define BIND_DECL(name)                                                        \
  \
public:                                                                        \
  \
void Bind##name(std::function<void()> input_command) {                         \
	m_##name = input_command;                                                  \
  }                                                                            \
  \
private:                                                                       \
  \
std::function<void()>                                                          \
	  m_##name;
class InputHandle
{
public:
	InputHandle() = default;
	void Initiate(u16 controller_ID);
	void CleanUp();
	void HandleInput();
	void Update();
	void Bind(u32 hash, std::function<void()> function);
private:
	ControllerInput* m_Controller = nullptr;
	InputWrapper* m_Input = nullptr;
	
	BIND_DECL(EscKey);
	BIND_DECL(TildeKey);
	BIND_DECL(1Key);
	BIND_DECL(2Key);
	BIND_DECL(3Key);
	BIND_DECL(4Key);
	BIND_DECL(5Key);
	BIND_DECL(6Key);
	BIND_DECL(7Key);
	BIND_DECL(8Key);
	BIND_DECL(9Key);
	BIND_DECL(0Key);
	BIND_DECL(DashKey);
	BIND_DECL(EqualKey);
	BIND_DECL(Backspace);
	BIND_DECL(TabKey);
	BIND_DECL(QKey);
	BIND_DECL(EKey);
	BIND_DECL(WKey);
	BIND_DECL(RKey);
	BIND_DECL(TKey);
	BIND_DECL(YKey);
	BIND_DECL(UKey);
	BIND_DECL(IKey);
	BIND_DECL(OKey);
	BIND_DECL(PKey);
	BIND_DECL(LBracketKey);
	BIND_DECL(RBracketKey);
	BIND_DECL(ReturnKey);
	BIND_DECL(LCtrlKey);
	BIND_DECL(AKey);
	BIND_DECL(SKey);
	BIND_DECL(DKey);
	BIND_DECL(FKey);
	BIND_DECL(GKey);
	BIND_DECL(HKey);
	BIND_DECL(JKey);
	BIND_DECL(KKey);
	BIND_DECL(LKey);
	BIND_DECL(SemicolonKey);
	BIND_DECL(ApostropheKey);
	BIND_DECL(GraveKey);
	BIND_DECL(LShiftKey);
	BIND_DECL(BackslashKey);
	BIND_DECL(ZKey);
	BIND_DECL(XKey);
	BIND_DECL(CKey);
	BIND_DECL(VKey);
	BIND_DECL(BKey);
	BIND_DECL(NKey);
	BIND_DECL(MKey);
	BIND_DECL(CommaKey);
	BIND_DECL(PeriodKey);
	BIND_DECL(SlashKey);
	BIND_DECL(RShiftKey);
	BIND_DECL(MultiplyKey);
	BIND_DECL(LAltKey);
	BIND_DECL(SpaceBar);
	BIND_DECL(CapsLock);
	BIND_DECL(F1Key);
	BIND_DECL(F2Key);
	BIND_DECL(F3Key);
	BIND_DECL(F4Key);
	BIND_DECL(F5Key);
	BIND_DECL(F6Key);
	BIND_DECL(F7Key);
	BIND_DECL(F8Key);
	BIND_DECL(F9Key);
	BIND_DECL(F10Key);
	BIND_DECL(F11Key);
	BIND_DECL(F12Key);
	BIND_DECL(F13Key);
	BIND_DECL(F14Key);
	BIND_DECL(F15Key);
	BIND_DECL(NumLock);
	BIND_DECL(Numpad1);
	BIND_DECL(Numpad2);
	BIND_DECL(Numpad3);
	BIND_DECL(Numpad4);
	BIND_DECL(Numpad5);
	BIND_DECL(Numpad6);
	BIND_DECL(Numpad7);
	BIND_DECL(Numpad8);
	BIND_DECL(Numpad9);
	BIND_DECL(Numpad0);
	BIND_DECL(NumpadAdd);
	BIND_DECL(NumpadMinus);
	BIND_DECL(NumpadMultiply);
	BIND_DECL(NumpadDecimal);
	BIND_DECL(NumpadDivide);
	BIND_DECL(ArrowUp);
	BIND_DECL(ArrowDown);
	BIND_DECL(ArrowLeft);
	BIND_DECL(ArrowRight);
	BIND_DECL(KeyDelete);
	BIND_DECL(InsertKey);
	BIND_DECL(PageDown);
	BIND_DECL(PageUp);
	BIND_DECL(HomeKey);
	BIND_DECL(EndKey);
	BIND_DECL(ScrollLock);
	BIND_DECL(PrintScreen);
	BIND_DECL(PauseBreak);
	BIND_DECL(XButton);
	BIND_DECL(YButton);
	BIND_DECL(BButton);
	BIND_DECL(AButton);
	BIND_DECL(LThumbYP);
	BIND_DECL(LThumbYN);
	BIND_DECL(LThumbXP);
	BIND_DECL(LThumbXN);
	BIND_DECL(RThumbYP);
	BIND_DECL(RThumbYN);
	BIND_DECL(RThumbXP);
	BIND_DECL(RThumbXN);
	BIND_DECL(RTrigger);
	BIND_DECL(RBumper);
	BIND_DECL(LTrigger);
	BIND_DECL(LBumper);
	BIND_DECL(DPadUp);
	BIND_DECL(DPadDown);
	BIND_DECL(DPadRight);
	BIND_DECL(DPadLeft);
};