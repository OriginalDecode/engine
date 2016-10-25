#include "../Engine/Engine.h"
#include "InputHandle.h"
#include "ControllerInput.h"
#include "InputWrapper.h"
#include "InputCommand.h"
#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#include <fstream>
#include <iostream>
#include <sstream>
void InputHandle::Initiate(u16 controller_ID)
{
	m_Controller = new ControllerInput(controller_ID);
	m_Input = new InputWrapper;
	Snowblind::CEngine* engine = Snowblind::CEngine::GetInstance();
	m_Input->Initiate(engine->GetWindow().GetHWND()
					, engine->GetWindow().GetWindowInstance());

	ClassifyBButton();
	ClassifyYButton();
	ClassifyXButton();
	ClassifyAButton();
	ClassifyWKey();
	ClassifyAKey();
	ClassifySKey();
	ClassifyDKey();
	ClassifySpaceBar();
	ClassifyLThumbXN();
	ClassifyLThumbXP();
	ClassifyLThumbYN();
	ClassifyLThumbYP();

	std::stringstream total_string;
	for (u32 i = 0; i < m_Names.size(); i++)
	{
		total_string << m_Names[i];
	}

	u32 m_TotalHash = Hash(total_string.str().c_str());

	bool updated = false;

	std::fstream file;
	file.open("../hashlist.h");
	if (file.good())
	{
		u32 hex_value;
		file.ignore(2);
		file >> std::hex >> hex_value;
		if (hex_value != m_TotalHash)
			updated = true;
		file.close();
	}

	if (updated)
	{
		std::ofstream out_file;
		out_file.open("../hashlist.h");
		out_file << "//0x" << std::hex << m_TotalHash << "\n";
		out_file.flush();
		out_file << "#pragma once\n";
		out_file.flush();
		out_file << "#include \"standard_datatype.hpp\"\n";
		out_file.flush();

		for (u32 i = 0; i < m_Names.size(); i++)
		{
			out_file 
				<< "static const u32 s_" << m_Names[i] << "_hash = "
				<< "0x" << std::hex << Hash(m_Names[i]) << ";\n";
			out_file.flush();
		}

		out_file.close();
	}
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
