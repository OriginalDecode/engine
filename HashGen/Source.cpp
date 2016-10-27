#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Hash.h"

//m_Names.push_back("XButton");
//m_Names.push_back("YButton");
//m_Names.push_back("BButton");
//m_Names.push_back("AButton");
//m_Names.push_back("WKey");
//m_Names.push_back("SKey");
//m_Names.push_back("AKey");
//m_Names.push_back("DKey");
//m_Names.push_back("SpaceBar");
//m_Names.push_back("LThumbYP");
//m_Names.push_back("LThumbYN");
//m_Names.push_back("LThumbXP");
//m_Names.push_back("LThumbXN");
#define PUSH_BACK(name) m_Names.push_back(name); 
int main(int argc, char* argv[])
{
	if (argv[1] == "-clear")
		remove("hashlist.h");


	std::cout << "Running Script\n";
	std::vector<const char*> m_Names;

	//Following American keyboard layout

	m_Names.push_back("EscKey");
	m_Names.push_back("TildeKey"); //Same as grave?
	m_Names.push_back("1Key");
	m_Names.push_back("2Key");
	m_Names.push_back("3Key");
	m_Names.push_back("4Key");
	m_Names.push_back("5Key");
	m_Names.push_back("6Key");
	m_Names.push_back("7Key");
	m_Names.push_back("8Key");
	m_Names.push_back("9Key");
	m_Names.push_back("0Key");
	m_Names.push_back("DashKey");
	m_Names.push_back("EqualKey");
	m_Names.push_back("Backspace");

	m_Names.push_back("TabKey");
	m_Names.push_back("QKey");
	m_Names.push_back("EKey");
	m_Names.push_back("WKey");
	m_Names.push_back("RKey");
	m_Names.push_back("TKey");
	m_Names.push_back("YKey");
	m_Names.push_back("UKey");
	m_Names.push_back("IKey");
	m_Names.push_back("OKey");
	m_Names.push_back("PKey");
	m_Names.push_back("LBracketKey");
	m_Names.push_back("RBracketKey");
	m_Names.push_back("ReturnKey");

	m_Names.push_back("LCtrlKey");
	m_Names.push_back("AKey");
	m_Names.push_back("SKey");
	m_Names.push_back("DKey");
	m_Names.push_back("FKey");
	m_Names.push_back("GKey");
	m_Names.push_back("HKey");
	m_Names.push_back("JKey");
	m_Names.push_back("KKey");
	m_Names.push_back("LKey");
	m_Names.push_back("SemicolonKey");
	m_Names.push_back("ApostropheKey");
	m_Names.push_back("GraveKey");


	m_Names.push_back("LShiftKey");
	m_Names.push_back("BackslashKey");
	m_Names.push_back("ZKey");
	m_Names.push_back("XKey");
	m_Names.push_back("CKey");
	m_Names.push_back("VKey");
	m_Names.push_back("BKey");
	m_Names.push_back("NKey");
	m_Names.push_back("MKey");

	m_Names.push_back("CommaKey");
	m_Names.push_back("PeriodKey");
	m_Names.push_back("SlashKey");
	m_Names.push_back("RShiftKey");
	m_Names.push_back("MultiplyKey");
	m_Names.push_back("LAltKey");
	m_Names.push_back("SpaceBar");
	m_Names.push_back("CapsLock");

	m_Names.push_back("F1Key");
	m_Names.push_back("F2Key");
	m_Names.push_back("F3Key");
	m_Names.push_back("F4Key");
	m_Names.push_back("F5Key");
	m_Names.push_back("F6Key");
	m_Names.push_back("F7Key");
	m_Names.push_back("F8Key");
	m_Names.push_back("F9Key");
	m_Names.push_back("F10Key");
	m_Names.push_back("F11Key");
	m_Names.push_back("F12Key");
	m_Names.push_back("F13Key");
	m_Names.push_back("F14Key");
	m_Names.push_back("F15Key");


	m_Names.push_back("NumLock");
	m_Names.push_back("Numpad1");
	m_Names.push_back("Numpad2");
	m_Names.push_back("Numpad3");
	m_Names.push_back("Numpad4");
	m_Names.push_back("Numpad5");
	m_Names.push_back("Numpad6");
	m_Names.push_back("Numpad7");
	m_Names.push_back("Numpad8");
	m_Names.push_back("Numpad9");
	m_Names.push_back("Numpad0");
	m_Names.push_back("NumpadAdd");
	m_Names.push_back("NumpadMinus");
	m_Names.push_back("NumpadMultiply");
	m_Names.push_back("NumpadDecimal");
	m_Names.push_back("NumpadDivide");

	m_Names.push_back("ArrowUp");
	m_Names.push_back("ArrowDown");
	m_Names.push_back("ArrowLeft");
	m_Names.push_back("ArrowRight");

	m_Names.push_back("DeleteKey");
	m_Names.push_back("InsertKey");
	m_Names.push_back("PageDown");
	m_Names.push_back("PageUp");
	m_Names.push_back("HomeKey");
	m_Names.push_back("EndKey");

	m_Names.push_back("ScrollLock");
	m_Names.push_back("PrintScreen");
	m_Names.push_back("PauseBreak");

	m_Names.push_back("XButton");
	m_Names.push_back("YButton");
	m_Names.push_back("BButton");
	m_Names.push_back("AButton");
	m_Names.push_back("LThumbYP");
	m_Names.push_back("LThumbYN");
	m_Names.push_back("LThumbXP");
	m_Names.push_back("LThumbXN");
	m_Names.push_back("RThumbYP");
	m_Names.push_back("RThumbYN");
	m_Names.push_back("RThumbXP");
	m_Names.push_back("RThumbXN");
	m_Names.push_back("RTrigger");
	m_Names.push_back("RBumper");
	m_Names.push_back("LTrigger");
	m_Names.push_back("LBumper");
	m_Names.push_back("DPadUp");
	m_Names.push_back("DPadDown");
	m_Names.push_back("DPadRight");
	m_Names.push_back("DPadLeft");

	std::stringstream total_string;
	std::cout << "Generating file hash.\n";
	for (unsigned int i = 0; i < m_Names.size(); i++)
	{
		total_string << m_Names[i];
	}

	int m_TotalHash = Hash(total_string.str().c_str());

	bool not_updated = true;

	float percentage = 5.f;
	const float toAdd = 5.f;
	u32 sizeofarray = m_Names.size();
	float dec = (float)sizeofarray / 100.f;
	std::fstream file;
	file.open("hashlist.h");
	if (file.good())
	{
		std::cout << "Checking if file has been updated.\n";
		int hex_value;
		file.ignore(2);
		file >> std::hex >> hex_value;
		if (hex_value != m_TotalHash)
		{
			std::cout << "File was updated.\n";
			not_updated = false;
		}
		file.close();
	}
	if (!not_updated)
	{
		float counter = 0.f;
		std::cout << "Creating new hashlist.\n";
		std::ofstream out_file;
		out_file.open("hashlist.h");
		out_file << "//0x" << std::hex << m_TotalHash << "\n";
		out_file.flush();
		out_file << "#pragma once\n";
		out_file.flush();
		out_file << "#include \"standard_datatype.hpp\"\n";
		out_file.flush();
		std::cout << "0%|";
		for (u32 i = 0; i < m_Names.size(); i++)
		{
			if (counter > percentage)
			{
				std::cout << "=";
				percentage += toAdd;
			}

			out_file
				<< "static const u32 s_" << m_Names[i] << "_hash = "
				<< "0x" << std::hex << Hash(m_Names[i]) << ";\n";
			out_file.flush();
			counter += dec;
		}
		std::cout << "|100%";
		std::cout << "\nTotal of " << sizeofarray << " hashes written to file.";
		std::cout << "\nFinished writing new hashes to file.\n";

		out_file.close();
		std::cout << "Closing file.\n";
	}
	if (!not_updated)
		std::cout << "Finished generating hashlist.h\n";
	else
		std::cout << "Failed to generate new hashlist.\n";
	return 0;
}
