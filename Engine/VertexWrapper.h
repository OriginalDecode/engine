#pragma once
#include <Engine/Engine.h>
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/VertexStructs.h>
class VertexWrapper
{
	friend class ModelImporter;
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
		SAFE_DELETE(m_Data);
	}

	s8* GetData() const { return m_Data; }
	s32 GetStart() const { return m_Start; }
	const u32& GetStride() const { return m_Stride; }
	const u32& GetByteOffset() const { return m_ByteOffset; }
	s32 GetVertexCount() const { return m_VertexCount; }
	s32 GetBufferCount() const { return m_BufferCount; }
	s32 GetSize() const { return m_Size; }
	IBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

	IInputLayout* GetInputLayout() const { return m_VertexInputLayout; }
	graphics::eTopology GetTopology() const { return m_Topology; }

	void SetVertexCount(const s32 vtx_count) { m_VertexCount = vtx_count; }

	void SetBuffer(IBuffer* buffer);
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

void VertexWrapper::SetBuffer(IBuffer* buffer)
{
	Engine::GetAPI()->ReleasePtr(m_VertexBuffer);
	m_VertexBuffer = buffer;
}
