#include "File.h"
#include <cstdio>
#include <cassert>
#include <memory>
#include <CommonLib/Utilities.h>
namespace Core
{
	File::File(const char* filepath, FileMode mode, int offset)
	{
		Open(filepath, mode, offset);
	}

	File::~File()
	{
		if (m_Mode == FileMode::WRITE_FILE)
		{
			if (FILE* hFile = fopen(m_Filepath, "wb"))
			{
				fwrite(m_Buffer, 1, m_FileSize, hFile);
				fclose(hFile);
			}
		}
		
		delete m_Buffer;
		m_Buffer = nullptr;
	}

	void File::Open(const char* filepath, FileMode mode, int offset /*= 0*/)
	{
		m_Mode = mode;
		m_Filepath = filepath;
		if (m_Mode == FileMode::READ_FILE)
			OpenForRead();
		else if (m_Mode == FileMode::WRITE_FILE)
			OpenForWrite();
		else
			assert(!"Failed to open file!");
	}

	void File::Write(const void* data, size_t element_size, size_t nof_elements)
	{
		if (m_FileSize + (element_size * nof_elements) > m_AllocatedSize)
		{
			const uint64 newSize = cl::nearest_Pow(m_FileSize + (element_size * nof_elements));

			char* buffer = new char[newSize];
			memcpy(&buffer[0], &m_Buffer[0], m_FileSize);
			m_AllocatedSize = newSize;
			delete[] m_Buffer;
			m_Buffer = nullptr;
			m_Buffer = buffer;
		}

		memcpy(&m_Buffer[m_FileSize], data, (element_size * nof_elements));
		m_FileSize += element_size * nof_elements;

	}

	void File::OpenForWrite()
	{
		constexpr int allocSize = 1024;
		m_Buffer = new char[allocSize];
		m_AllocatedSize = allocSize;
	}

	void File::OpenForRead()
	{
		if (FILE* hFile = fopen(m_Filepath, "rb"))
		{
			fseek(hFile, 0, SEEK_END);
			m_FileSize = ftell(hFile);
			rewind(hFile);

			m_Buffer = new char[m_FileSize];
			memset(m_Buffer,0 , m_FileSize);

			fread(m_Buffer, 1, m_FileSize, hFile);
			fclose(hFile);
		}
	}

};