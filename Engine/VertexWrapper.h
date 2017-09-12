#pragma once
#include "VertexTypes.h"
#include <Engine/engine_shared.h>

// struct VertexDataWrapper
// {
// 	~VertexDataWrapper() { SAFE_DELETE(myVertexData); };
// 	s8* myVertexData = nullptr;
// 	s32 myNrOfVertexes = 0;
// 	s32 mySize = 0;
// 	s32 myStride = 0;
// };
// 
// struct VertexBufferWrapper
// {
// 	~VertexBufferWrapper() { SAFE_RELEASE(myVertexBuffer); };
// 	ID3D11Buffer* myVertexBuffer = nullptr;
// 	s32 myStartSlot = 0;
// 	s32 myNrOfBuffers = 0;
// 	u32 myStride = 0;
// 	u32 myByteOffset = 0;
// };

class VertexWrapper
{
public:
	VertexWrapper(s8* data, s32 vertex_start, s32 buffer_count, s32 stride, s32 byte_offset, s32 vertex_count, s32 size, IBuffer* vertex_buffer, IInputLayout* input_layout, graphics::eTopology topology)
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
	s32 GetStride() const { return m_Stride; }
	s32 GetByteOffset() const { return m_ByteOffset; }
	s32 GetVertexCount() const { return m_VertexCount; }
	s32 GetBufferCount() const { return m_BufferCount; }
	s32 GetSize() const { return m_Size; }
	IBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

	IInputLayout* GetInputLayout() const { return m_VertexInputLayout; }
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
