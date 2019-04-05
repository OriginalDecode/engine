#include "File.h"
#include <cstdio>
#include <cassert>
namespace Core
{
	File::File(const char* filepath, FileMode mode, int offset)
	{

		char* fileMode = { 0 };
		if (mode == FileMode::READ_FILE)
			fileMode = "rb";
		else if (mode == FileMode::WRITE_FILE)
			fileMode = "wb";

		if (m_FileHandle = fopen(filepath, fileMode))
		{
			fseek(m_FileHandle, 0, SEEK_END);
			m_FileSize = ftell(m_FileHandle);
			rewind(m_FileHandle);

			m_Buffer = new char[m_FileSize];

			fread(m_Buffer, 1, m_FileSize, m_FileHandle);
		}
		else
		{
			assert(!"Failed to open file!");
		}

	}

	File::~File()
	{
		fclose(m_FileHandle);
		delete m_Buffer;
		m_Buffer = nullptr;
	}

	void File::Write(const void* data, int element_size, int length)
	{
		fwrite(data, element_size, length, m_FileHandle);
	}

};