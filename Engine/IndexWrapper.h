#pragma once
#include <Engine/Engine.h>
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>

class IndexWrapper
{
	friend class ModelImporter;
public:
	IndexWrapper() = default;
	IndexWrapper(int8* data, int32 index_count, int32 start, int32 size, graphics::eTextureFormat format, int32 byte_offset, IBuffer* buffer)
		: m_Data(data)
		, m_IndexCount(index_count)
		, m_Start(start)
		, m_Size(size)
		, m_IndexBufferFormat(format)
		, m_ByteOffset(byte_offset)
		, m_IndexBuffer(buffer)
	{
	}

	~IndexWrapper()
	{
		Reset();
	}

	void Reset()
	{
		if (graphics::IGraphicsAPI* api = Engine::GetAPI())
			Engine::GetAPI()->ReleasePtr(m_IndexBuffer);
		delete[] m_Data;
	}

	void SetData(int8* data) { m_Data = data; }
	int8* GetData() const { return m_Data; }

	void SetIndexCount(int32 index_count) { m_IndexCount = index_count; }
	int32 GetIndexCount() const { return m_IndexCount; }

	void SetSize(int32 size) { m_Size = size; }
	int32 GetSize() const { return m_Size; }

	void SetFormat(graphics::eTextureFormat format) { m_IndexBufferFormat = format; }
	graphics::eTextureFormat GetFormat() const { return m_IndexBufferFormat; }

	void SetByteOffset(int32 byte_offset) { m_ByteOffset = byte_offset; }
	int32 GetByteOffset() const { return m_ByteOffset; }

	void SetStart(int32 start) { m_Start = start; }
	int32 GetStart() const { return m_Start; }


	IBuffer* GetIndexBuffer() { return m_IndexBuffer; }
	void SetBuffer(IBuffer* buffer);

	void ReleaseBuffer();

#ifdef _DEBUG
	std::string m_DebugName;
#endif

private:

	int8* m_Data = nullptr;
	int32 m_IndexCount = 0;
	int32 m_Size = 0;
	int32 m_Start = 0;
	graphics::eTextureFormat m_IndexBufferFormat;
	int32 m_ByteOffset = 0;
	IBuffer* m_IndexBuffer = nullptr;

};

inline void IndexWrapper::SetBuffer(IBuffer* buffer)
{
	m_IndexBuffer = buffer;
}

inline void IndexWrapper::ReleaseBuffer()
{
	Engine::GetAPI()->ReleasePtr(m_IndexBuffer);
}

