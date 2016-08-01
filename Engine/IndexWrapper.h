#pragma once
#include "EngineDefines.h"
#include <standard_datatype.hpp>

enum DXGI_FORMAT;
struct ID3D11Buffer;

namespace Snowblind
{
	struct SVertexIndexWrapper
	{
		~SVertexIndexWrapper() { SAFE_DELETE(myIndexData); };
		s8* myIndexData;
		s32 myIndexCount;
		s32 mySize;
		DXGI_FORMAT myFormat;
	};

	struct SIndexBufferWrapper
	{
		~SIndexBufferWrapper() { SAFE_RELEASE(myIndexBuffer); };
		ID3D11Buffer* myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
		u16 myByteOffset;
	};
};