#pragma once


namespace Core
{
	enum class FileMode
	{
		NONE,
		READ_FILE,
		WRITE_FILE
	};

	class File
	{
	public:
		File(const char* filepath, FileMode mode = FileMode::READ_FILE, int offset = 0);
		~File();
		
		int GetSize() const { return m_FileSize; }
		const char* const GetBuffer() const { return m_Buffer; }
		void Write(const void* data, int element_size, int length);
	private:
		FileMode m_Mode = FileMode::NONE;
		FILE* m_FileHandle = nullptr;
		char* m_Buffer = nullptr;
		int m_FileSize = 0;
	};


};