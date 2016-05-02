#pragma once
#include "VertexTypes.h"
struct ID3D11Buffer;


namespace Snowblind
{
	struct SVertexDataWrapper
	{
		~SVertexDataWrapper(){ delete myVertexData; myVertexData = nullptr; };
		char* myVertexData;
		int myNrOfVertexes;
		int mySize;
		int myStride;
	};

	struct SVertexBufferWrapper
	{
		~SVertexBufferWrapper(){ myVertexBuffer->Release(); myVertexBuffer = nullptr; };
		ID3D11Buffer* myVertexBuffer;
		int myStartSlot;
		int myNrOfBuffers;
		unsigned int myStride;
		unsigned int myByteOffset;
	};
};