#pragma once
#include <Engine/engine_shared.h>


class InstanceWrapper
{
	friend class ModelImporter;
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

	~InstanceWrapper()
	{
		Reset();
	}

	void Reset()
	{
		Engine::GetAPI()->ReleasePtr(m_InstanceInputLayout);
		m_InstanceInputLayout = nullptr;
		Engine::GetAPI()->ReleasePtr(m_InstanceBuffer);
		m_InstanceBuffer = nullptr;
	}


	void SetInstanceCount(s32 instance_count) { m_InstanceCount = instance_count; }
	s32 GetInstanceCount() const { return m_InstanceCount; }

	void SetVertexCountPerInstance(s32 vertices_per_instance ) { m_VerticesPerInstance = vertices_per_instance; }
	s32 GetVertCountPerInstance() const { return m_VerticesPerInstance; }

	void SetIndexCountPerInstance(s32 indices_per_instance) { m_IndicesPerInstance = indices_per_instance; }
	s32 GetIndexCountPerInstance() const { return m_IndicesPerInstance; }

	void SetByteOffset(s32 byte_offset) { m_ByteOffset = byte_offset; }
	s32 GetByteOffset() const { return m_ByteOffset; }

	void SetStride(s32 stride) { m_Stride = stride; }
	s32 GetStride() const { return m_Stride; }

	void SetStart(s32 start) { m_Start = start; }
	s32 GetStart() const { return m_Start; }

	void SetBufferCount(s32 buffer_count) { m_BufferCount = buffer_count; }
	s32 GetBufferCount() const { return m_BufferCount; }

	void SetSize(s32 size) { m_Size = size; }
	s32 GetSize() const { return m_Size; }

	IBuffer* GetInstanceBuffer() { return m_InstanceBuffer; }
	void SetBuffer(IBuffer* buffer) { m_InstanceBuffer = buffer; }
	void ReleaseBuffer() { Engine::GetAPI()->ReleasePtr(m_InstanceBuffer); }

	void SetInputLayout(IInputLayout* input_layout) { m_InstanceInputLayout = input_layout; }
	IInputLayout* GetInputLayout() { return m_InstanceInputLayout; }


#ifdef _DEBUG
	std::string m_DebugName;
#endif
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
	s32 m_Size = 0;
	IBuffer* m_InstanceBuffer = nullptr;
	IInputLayout* m_InstanceInputLayout = nullptr;
};