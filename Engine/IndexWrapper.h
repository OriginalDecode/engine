#pragma once
#include <Engine/Engine.h>
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>

class IndexWrapper
{
	friend class ModelImporter;
public:
	IndexWrapper() = default;
	IndexWrapper(s8* data, s32 index_count, s32 start, s32 size, graphics::eTextureFormat format, s32 byte_offset, IBuffer* buffer)
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
		Engine::GetAPI()->ReleasePtr(m_IndexBuffer);
		delete[] m_Data;
	}

	void SetData(s8* data) { m_Data = data; }
	s8* GetData() const { return m_Data; }

	void SetIndexCount(s32 index_count) { m_IndexCount = index_count; }
	s32 GetIndexCount() const { return m_IndexCount; }

	void SetSize(s32 size) { m_Size = size; }
	s32 GetSize() const { return m_Size; }

	void SetFormat(graphics::eTextureFormat format) { m_IndexBufferFormat = format; }
	graphics::eTextureFormat GetFormat() const { return m_IndexBufferFormat; }

	void SetByteOffset(s32 byte_offset) { m_ByteOffset = byte_offset; }
	s32 GetByteOffset() const { return m_ByteOffset; }

	void SetStart(s32 start) { m_Start = start; }
	s32 GetStart() const { return m_Start; }


	IBuffer* GetIndexBuffer() { return m_IndexBuffer; }
	void SetBuffer(IBuffer* buffer);

	void ReleaseBuffer();

#ifdef _DEBUG
	std::string m_DebugName;
#endif

private:

	s8* m_Data = nullptr;
	s32 m_IndexCount = 0;
	s32 m_Size = 0;
	s32 m_Start = 0;
	graphics::eTextureFormat m_IndexBufferFormat;
	s32 m_ByteOffset = 0;
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

