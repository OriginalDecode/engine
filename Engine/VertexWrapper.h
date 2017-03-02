#pragma once
#include "VertexTypes.h"
#include "EngineDefines.h"
#include <standard_datatype.hpp>
struct ID3D11Buffer;



struct VertexDataWrapper
{
	~VertexDataWrapper() { SAFE_DELETE(myVertexData); };
	s8* myVertexData = nullptr;
	s32 myNrOfVertexes = 0;
	s32 mySize = 0;
	s32 myStride = 0;
};

struct VertexBufferWrapper
{
	~VertexBufferWrapper() { SAFE_RELEASE(myVertexBuffer); };
	ID3D11Buffer* myVertexBuffer = nullptr;
	s32 myStartSlot = 0;
	s32 myNrOfBuffers = 0;
	u32 myStride = 0;
	u32 myByteOffset = 0;
};
