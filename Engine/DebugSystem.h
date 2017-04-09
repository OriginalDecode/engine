#pragma once
#include "snowblind_shared.h"
#include <sstream>
#include <functional>
#include <vector>
#include <deque>

class InputHandle;
class Engine;
class Synchronizer;

class DebugPrinter
{
public:
	DebugPrinter() = default;
	bool Initiate(InputHandle* input_handle);
	void Update();
	void Render();
	void AddToErrorList(std::string error);
	void AddToDebugText(std::string debug_text);
private:
	std::deque<std::string> m_ErrorMessages;
	std::vector<std::string> m_DebugStrings;
	Engine* m_Engine = nullptr;
	Synchronizer* m_Synchronizer = nullptr;
	Ticket_Mutex m_Mutex;

};
