#pragma once
#include "EngineDefines.h"

enum DXGI_FORMAT;
struct ID3D11Buffer;

namespace Snowblind
{
	struct SVertexIndexWrapper
	{
		~SVertexIndexWrapper() { SAFE_DELETE(myIndexData); };
		char* myIndexData;
		int myIndexCount;
		int mySize;
		DXGI_FORMAT myFormat;
	};

	struct SIndexBufferWrapper
	{
		~SIndexBufferWrapper() { SAFE_RELEASE(myIndexBuffer); };
		ID3D11Buffer* myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
		int myByteOffset;
	};
};