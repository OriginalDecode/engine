#pragma once
#include "Engine.h"
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/VertexStructs.h>

class VertexWrapper
{
	friend class CModelImporter;
public:
	VertexWrapper() = default;
	VertexWrapper(int8* data,
				  int32 vertex_start, 
				  int32 buffer_count, 
				  int32 stride, 
				  int32 byte_offset, 
				  int32 vertex_count, 
				  int32 size, 
				  IBuffer* vertex_buffer, 
				  IInputLayout* input_layout, 
				  graphics::eTopology topology)
		: m_Data(data)
		, m_Start(vertex_start)
		, m_BufferCount(buffer_count)
		, m_Stride(stride)
		, m_ByteOffset(byte_offset)
		, m_VertexCount(vertex_count)
		, m_Size(size)
		, m_VertexBuffer(vertex_buffer)
		, m_VertexInputLayout(input_layout)
		, m_Topology(topology)
	{
	}

	~VertexWrapper()
	{
		Reset();
	}

	void Reset()
	{
		if (graphics::IGraphicsAPI* api = Engine::GetAPI())
		{
			api->ReleasePtr(m_VertexBuffer);
			m_VertexBuffer = nullptr;
			api->ReleasePtr(m_VertexInputLayout);
			m_VertexInputLayout = nullptr;
		}
		delete[] m_Data;
	}

	void SetData(int8* data) { m_Data = data; }
	int8* GetData() const { return m_Data; }

	void SetStart(int32 start) { m_Start = start; }
	int32 GetStart() const { return m_Start; }

	void SetStride(int32 stride) { m_Stride = stride; }
	const uint32& GetStride() const { return m_Stride; }

	void SetByteOffset(int32 byte_offset) { m_ByteOffset = byte_offset; }
	const uint32& GetByteOffset() const { return m_ByteOffset; }

	void SetVertexCount(int32 vertex_count) { m_VertexCount = vertex_count; }
	int32 GetVertexCount() const { return m_VertexCount; }

	int32 GetBufferCount() const { return m_BufferCount; }

	void SetSize(int32 size) { m_Size = size; }
	int32 GetSize() const { return m_Size; }

	void SetBuffer(IBuffer* buffer);
	IBuffer* GetVertexBuffer() { return m_VertexBuffer; }
	void ReleaseBuffer();

	void SetInputLayout(IInputLayout* layout) { m_VertexInputLayout = layout; }
	IInputLayout* GetInputLayout() { return m_VertexInputLayout; }

	void SetTopology(graphics::eTopology topology) { m_Topology = topology; }
	graphics::eTopology GetTopology() const { return m_Topology; }

#ifdef _DEBUG
	std::string m_DebugName;
#endif

private:

	int8* m_Data = nullptr;
	int32 m_Start = 0;
	int32 m_BufferCount = 0;
	int32 m_Stride = 0;
	int32 m_ByteOffset = 0;
	int32 m_VertexCount = 0;
	int32 m_Size = 0;
	IBuffer* m_VertexBuffer = nullptr;
	IInputLayout* m_VertexInputLayout = nullptr;
	graphics::eTopology m_Topology = graphics::TRIANGLE_LIST;
};

inline void VertexWrapper::SetBuffer(IBuffer* buffer)
{
	m_VertexBuffer = buffer;
	m_BufferCount++;
}

inline void VertexWrapper::ReleaseBuffer()
{
	
	Engine::GetAPI()->ReleasePtr(m_VertexBuffer);
	if(m_BufferCount > 0)
		m_BufferCount--;
}
