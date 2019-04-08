#pragma once
#include <Engine/engine_shared.h>
#include <Engine/Engine.h>
#include <Engine/IGraphicsAPI.h>

class InstanceWrapper
{
	friend class ModelImporter;
public:
	InstanceWrapper() = default;
	InstanceWrapper(int32 instance_count, int32 points_per_instance, int32 byte_offset, int32 stride, int32 start_index, int32 buffer_count, IBuffer* buffer, IInputLayout* input_layout )
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
		if (graphics::IGraphicsAPI* api = Engine::GetAPI())
		{
			api->ReleasePtr(m_InstanceInputLayout);
			m_InstanceInputLayout = nullptr;
			api->ReleasePtr(m_InstanceBuffer);
			m_InstanceBuffer = nullptr;
		}
	}


	void SetInstanceCount(int32 instance_count) { m_InstanceCount = instance_count; }
	int32 GetInstanceCount() const { return m_InstanceCount; }

	void SetVertexCountPerInstance(int32 vertices_per_instance ) { m_VerticesPerInstance = vertices_per_instance; }
	int32 GetVertCountPerInstance() const { return m_VerticesPerInstance; }

	void SetIndexCountPerInstance(int32 indices_per_instance) { m_IndicesPerInstance = indices_per_instance; }
	int32 GetIndexCountPerInstance() const { return m_IndicesPerInstance; }

	void SetByteOffset(int32 byte_offset) { m_ByteOffset = byte_offset; }
	int32 GetByteOffset() const { return m_ByteOffset; }

	void SetStride(int32 stride) { m_Stride = stride; }
	int32 GetStride() const { return m_Stride; }

	void SetStart(int32 start) { m_Start = start; }
	int32 GetStart() const { return m_Start; }

	void SetBufferCount(int32 buffer_count) { m_BufferCount = buffer_count; }
	int32 GetBufferCount() const { return m_BufferCount; }

	void SetSize(int32 size) { m_Size = size; }
	int32 GetSize() const { return m_Size; }

	IBuffer* GetInstanceBuffer() { return m_InstanceBuffer; }
	void SetBuffer(IBuffer* buffer) { m_InstanceBuffer = buffer; }
	void ReleaseBuffer() { Engine::GetAPI()->ReleasePtr(m_InstanceBuffer); }

	void SetInputLayout(IInputLayout* input_layout) { m_InstanceInputLayout = input_layout; }
	IInputLayout* GetInputLayout() { return m_InstanceInputLayout; }


#ifdef _DEBUG
	std::string m_DebugName;
#endif
private:

	int32 m_InstanceCount = 0;
	union
	{
		int32 m_VerticesPerInstance = 0;
		int32 m_IndicesPerInstance;
	};

	int32 m_ByteOffset = 0;
	int32 m_Stride = 0;

	int32 m_Start = 0;
	int32 m_BufferCount = 0;
	int32 m_Size = 0;
	IBuffer* m_InstanceBuffer = nullptr;
	IInputLayout* m_InstanceInputLayout = nullptr;
};