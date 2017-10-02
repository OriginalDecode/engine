#pragma once
#include <Engine/Engine.h>
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/VertexStructs.h>
class VertexWrapper
{
	friend class CModelImporter;
public:
	VertexWrapper() = default;
	VertexWrapper(s8* data,
				  s32 vertex_start, 
				  s32 buffer_count, 
				  s32 stride, 
				  s32 byte_offset, 
				  s32 vertex_count, 
				  s32 size, 
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
		Engine::GetAPI()->ReleasePtr(m_VertexBuffer);
		Engine::GetAPI()->ReleasePtr(m_VertexInputLayout);
		SAFE_DELETE(m_Data);
	}

	void SetData(s8* data) { m_Data = data; }
	s8* GetData() const { return m_Data; }

	void SetStart(s32 start) { m_Start = start; }
	s32 GetStart() const { return m_Start; }

	void SetStride(s32 stride) { m_Stride = stride; }
	const u32& GetStride() const { return m_Stride; }

	void SetByteOffset(s32 byte_offset) { m_ByteOffset = byte_offset; }
	const u32& GetByteOffset() const { return m_ByteOffset; }

	void SetVertexCount(s32 vertex_count) { m_VertexCount = vertex_count; }
	s32 GetVertexCount() const { return m_VertexCount; }

	void SetBufferCount(s32 count) { m_BufferCount = count; }
	s32 GetBufferCount() const { return m_BufferCount; }

	void SetSize(s32 size) { m_Size = size; }
	s32 GetSize() const { return m_Size; }

	void SetBuffer(IBuffer* buffer);
	IBuffer* GetVertexBuffer() { return m_VertexBuffer; }
	void ReleaseBuffer();

	void SetInputLayout(IInputLayout* layout) { m_VertexInputLayout = layout; }
	IInputLayout* GetInputLayout() { return m_VertexInputLayout; }

	void SetTopology(graphics::eTopology topology) { m_Topology = topology; }
	graphics::eTopology GetTopology() const { return m_Topology; }


private:

	s8* m_Data = nullptr;
	s32 m_Start = 0;
	s32 m_BufferCount = 0;
	s32 m_Stride = 0;
	s32 m_ByteOffset = 0;
	s32 m_VertexCount = 0;
	s32 m_Size = 0;
	IBuffer* m_VertexBuffer = nullptr;
	IInputLayout* m_VertexInputLayout = nullptr;
	graphics::eTopology m_Topology = graphics::TRIANGLE_LIST;
};

inline void VertexWrapper::SetBuffer(IBuffer* buffer)
{
	m_VertexBuffer = buffer;
}

inline void VertexWrapper::ReleaseBuffer()
{
	Engine::GetAPI()->ReleasePtr(m_VertexBuffer);
}
