#pragma once
#include "EngineDefines.h"
#include <standard_datatype.hpp>

#ifndef SNOWBLIND_VULKAN
enum DXGI_FORMAT;
struct ID3D11Buffer;
#endif
namespace Snowblind
{
	struct SVertexIndexWrapper
	{
		~SVertexIndexWrapper() { SAFE_DELETE(myIndexData); };
		s8* myIndexData;
		s32 myIndexCount;
		s32 mySize;
#ifndef SNOWBLIND_VULKAN
		DXGI_FORMAT myFormat;
#endif
	};

	struct SIndexBufferWrapper
	{
		//~SIndexBufferWrapper() { SAFE_RELEASE(myIndexBuffer); };
#ifndef SNOWBLIND_VULKAN
		ID3D11Buffer* myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
#endif
		u16 myByteOffset;
	};
};