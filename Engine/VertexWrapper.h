#pragma once
#include "VertexTypes.h"
#include "EngineDefines.h"
#include <standard_datatype.hpp>
struct ID3D11Buffer;


namespace Snowblind
{
	struct VertexDataWrapper
	{
		~VertexDataWrapper() { SAFE_DELETE(myVertexData); };
		s8* myVertexData;
		s32 myNrOfVertexes;
		s32 mySize;
		s32 myStride;
	};

	struct VertexBufferWrapper
	{
		~VertexBufferWrapper(){ SAFE_RELEASE(myVertexBuffer); };
		ID3D11Buffer* myVertexBuffer = nullptr;
		s32 myStartSlot;
		s32 myNrOfBuffers;
		u32 myStride;
		u32 myByteOffset;
	};
};