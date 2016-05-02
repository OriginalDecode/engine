#pragma once
#include "VertexTypes.h"
#include "EngineDefines.h"
struct ID3D11Buffer;


namespace Snowblind
{
	struct SVertexDataWrapper
	{
		~SVertexDataWrapper() { SAFE_DELETE(myVertexData); };
		char* myVertexData;
		int myNrOfVertexes;
		int mySize;
		int myStride;
	};

	struct SVertexBufferWrapper
	{
		~SVertexBufferWrapper(){ SAFE_RELEASE(myVertexBuffer); };
		ID3D11Buffer* myVertexBuffer;
		int myStartSlot;
		int myNrOfBuffers;
		unsigned int myStride;
		unsigned int myByteOffset;
	};
};