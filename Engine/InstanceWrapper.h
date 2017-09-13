#pragma once
#include <Engine/engine_shared.h>

class InstanceWrapper
{
public:
	InstanceWrapper() = default;
	InstanceWrapper(s32 instance_count, s32 points_per_instance, s32 byte_offset, s32 stride, s32 start_index, s32 buffer_count, IBuffer* buffer, IInputLayout* input_layout )
		: m_InstanceCount(instance_count)
		, m_VerticesPerInstance(points_per_instance)
		, m_ByteOffset(byte_offset)
		, m_Stride(stride)
		, m_Start(start_index)
		, m_BufferCount(buffer_count)
		, m_InstanceBuffer(buffer)
		, m_InstanceInputLayout(input_layout)
	{
	}

	s32 GetInstanceCount() const { return m_InstanceCount; }
	s32 GetVertCountPerInstance() const { return m_VerticesPerInstance; }
	s32 GetIndexCountPerInstance() const { return m_IndicesPerInstance; }
	s32 GetByteOffset() const { return m_ByteOffset; }
	s32 GetStride() const { return m_Stride; }
	s32 GetStart() const { return m_Start; }
	s32 GetBufferCount() const { return m_BufferCount; }
	IBuffer* GetInstanceBuffer() const { return m_InstanceBuffer; }

	IInputLayout* GetInputLayout() const { return m_InstanceInputLayout; }



private:

	s32 m_InstanceCount = 0;
	union
	{
		s32 m_VerticesPerInstance = 0;
		s32 m_IndicesPerInstance;
	};

	s32 m_ByteOffset = 0;
	s32 m_Stride = 0;

	s32 m_Start = 0;
	s32 m_BufferCount = 0;
	IBuffer* m_InstanceBuffer = nullptr;
	IInputLayout* m_InstanceInputLayout = nullptr;
};