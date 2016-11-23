#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Hash.h"
#include "CommonLib/Utilities.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> m_File; //If it already exists
	std::vector<std::string> m_Names;

	const char* output_file = "hashlist.h";

	std::fstream to_hash("Keys_To_Hash.tohash");
	std::string str;
	if (to_hash.good())
	{
		while (!to_hash.eof())
		{
			std::getline(to_hash, str);
			m_Names.push_back(str);
		}
	}

	std::fstream hash_file(output_file);
	if (hash_file.good())
	{
		while (!hash_file.eof())
		{
			std::getline(hash_file, str);
			if (str.find("#") != str.npos)
				continue;
			if (str.find("//") != str.npos)
				continue;

			m_File.push_back(str);
		}
	}

	std::stringstream total_string;
	std::cout << "Generating file hash.\n";
	for (unsigned int i = 0; i < m_Names.size(); i++)
	{
		total_string << m_Names[i];
	}

	int file_to_hash = Hash(total_string.str().c_str());

	std::stringstream total_string_comp;
	for (unsigned int i = 0; i < m_File.size(); i++)
	{
		total_string_comp << m_File[i];
	}

	int exists_file_hash = Hash(total_string_comp.str().c_str());



	bool not_updated = true;

	std::fstream file;
	file.open(output_file);
	if (file.good())
	{
		std::cout << "Checking if file has been updated.\n";
		int hex_value;
		file.ignore(2);
		file >> std::hex >> hex_value;
		if (exists_file_hash != file_to_hash)
		{
			std::cout << "File was updated.\n";
			not_updated = false;
		}
		file.close();
	}
	else
	{
		std::cout << "File was not updated\n";
		return 0;
	}

	if (!not_updated)
	{
		std::cout << "Creating new hashlist.\n";
		std::ofstream out_file;
		out_file.open("hashlist.h");
		out_file << "//0x" << std::hex << file_to_hash << "\n";
		out_file.flush();
		out_file << "#pragma once\n";
		out_file.flush();
		out_file << "#include \"standard_datatype.hpp\"\n";
		out_file.flush();
		for (u32 i = 0; i < m_Names.size(); i++)
		{
			out_file
				<< "static const u32 s_" << m_Names[i] << "_hash = "
				<< "0x" << std::hex << Hash(m_Names[i].c_str()) << ";\n";
			out_file.flush();
		}
		std::cout << "\nTotal of " << m_Names.size() << " hashes written to file.";
		std::cout << "\nFinished writing new hashes to file.\n";

		out_file.close();
		std::cout << "Closing file.\n";
	}
	std::cout << "Done\n";
	return 0;
}
