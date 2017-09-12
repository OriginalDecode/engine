#pragma once
#include <Engine/engine_shared.h>

class InstanceWrapper
{
public:
	InstanceWrapper(s32 instance_count, s32 points_per_instance, u32* byte_offsets, u32* strides, s32 start_index, s32 buffer_count, IBuffer* buffers, IInputLayout* input_layout )
		: m_InstanceCount(instance_count)
		, m_VerticesPerInstance(points_per_instance)
		, m_ByteOffsets(byte_offsets)
		, m_Strides(strides)
		, m_Start(start_index)
		, m_BufferCount(buffer_count)
		, m_InstanceBuffers(buffers)
		, m_InstanceInputLayout(input_layout)
	{
	}

	s32 GetInstanceCount() const { return m_InstanceCount; }
	s32 GetVertCountPerInstance() const { return m_VerticesPerInstance; }
	s32 GetIndexCountPerInstance() const { return m_IndicesPerInstance; }
	u32* GetByteOffsets() const { return m_ByteOffsets; }
	u32* GetStrides() const { return m_Strides; }
	s32 GetStart() const { return m_Start; }
	s32 GetBufferCount() const { return m_BufferCount; }
	IBuffer* GetInstanceBuffers() const { return m_InstanceBuffers; }

	IInputLayout* GetInputLayout() const { return m_InstanceInputLayout; }

private:

	s32 m_InstanceCount = 0;
	union
	{
		s32 m_VerticesPerInstance = 0;
		s32 m_IndicesPerInstance;
	};

	u32* m_ByteOffsets = nullptr;
	u32* m_Strides = nullptr;

	s32 m_Start = 0;
	s32 m_BufferCount = 0;
	IBuffer* m_InstanceBuffers = nullptr;
	IInputLayout* m_InstanceInputLayout = nullptr;
};