#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Hash.h"
#include "CommonLib/Utilities.h"

std::string CreateCase(const std::string& hash_name)
{
	std::stringstream to_output;
	to_output << "	case s_" << hash_name << "_hash:\n	{\n		Bind" << hash_name << "(function);\n	}break;\n";
	return to_output.str();
}

std::string CreateDecl(const std::string& hash_name)
{
	std::stringstream to_output;
	to_output << "	BIND_DECL(" << hash_name << ");";
	return to_output.str();
}

#define PUSH_BACK(name) m_Names.push_back(name); 
int main(int argc, char* argv[])
{
	std::cout << "Running Script\n";
	std::vector<std::string> m_Names;
	
	std::fstream hash_file("Keys_To_Hash.tohash");
	std::string str;
	if (hash_file.good())
	{
		while (!hash_file.eof())
		{
			std::getline(hash_file, str);
			m_Names.push_back(str);
		}
	}

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
	else
		not_updated = false;
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
				<< "0x" << std::hex << Hash(m_Names[i].c_str()) << ";\n";
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

	if (!not_updated)
	{
		std::string line;
		std::fstream file;
		file.open("Input/InputHandle.h");
		if (file.good())
		{
			std::vector<std::string> m_File;
			while (!file.eof())
			{
				std::string str;
				std::getline(file, str);
				m_File.push_back(str + "\n");
				if (str == "\t")
				{
					break;
				}
			}

			int index = 0;
			for(int i = 0; i < m_Names.size(); i++)
			{
				m_File.push_back(CreateDecl(m_Names[i]) + "\n");
			}
			m_File.push_back("};");
			
			std::ofstream out_file;
			out_file.open("Input/InputHandle.h");
			for (const std::string& line : m_File)
			{
				out_file << line;
				out_file.flush();
			}
			out_file.close();
		}
	}

	if (!not_updated)
	{
		std::string line;
		std::fstream file;
		file.open("Input/InputHandle.cpp");
		if (file.good())
		{
			std::vector<std::string> m_File;
			while (!file.eof())
			{
				std::string str;
				std::getline(file, str);
				m_File.push_back(str + "\n");
				if (str.find("Bind") != str.npos)
				{
					break;
				}
			}
			m_File.push_back("{\n");
			m_File.push_back("switch(hash)\n{\n");



			int index = 0;
			for (int i = 0; i < m_Names.size(); i++)
			{
				m_File.push_back(CreateCase(m_Names[i]));
			}

			std::ofstream out_file;
			out_file.open("Input/InputHandle.cpp");
			for (const std::string& line : m_File)
			{
				out_file << line;
				out_file.flush();
			}
			out_file << "	}\n}\n";
			out_file.flush();
			out_file.close();
		}
	}
	return 0;
}
