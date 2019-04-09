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

		void Open(const char* filepath, FileMode mode, int offset = 0);

		int GetSize() const { return m_FileSize; }
		const char* const GetBuffer() const { return m_Buffer; }
		void Write(const void* data, size_t element_size, size_t nof_elements);
	private:

		void OpenForWrite();
		void OpenForRead();
		const char* m_Filepath{ 0 };
		FileMode m_Mode = FileMode::NONE;
		char* m_Buffer = nullptr;
		int m_FileSize = 0;
		int m_AllocatedSize = 0;
	};


};