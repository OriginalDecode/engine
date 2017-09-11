#pragma once
#include <Engine/engine_shared.h>

class InstanceWrapper
{
public:
	InstanceWrapper() = default;

	s32 GetInstanceCount() const { return m_InstanceCount; }
	s32 GetVertCountPerInstance() const { return m_VerticesPerInstance; }
	s32 GetIndexCountPerInstance() const { return m_IndicesPerInstance; }
	s32 GetByteOffset() const { return m_ByteOffset; }
	s32 GetStart() const { return m_Start; }
	IBuffer* GetInstanceBuffer() const { return m_InstanceBuffer; }
private:

	s32 m_InstanceCount = 0;
	union
	{
		s32 m_VerticesPerInstance = 0;
		s32 m_IndicesPerInstance = 0;
	};
	s32 m_ByteOffset = 0;
	s32 m_Start = 0;
	IBuffer* m_InstanceBuffer = nullptr;

};