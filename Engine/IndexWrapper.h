#pragma once
#include "EngineDefines.h"
#include <standard_datatype.hpp>

enum DXGI_FORMAT;
struct ID3D11Buffer;

struct VertexIndexWrapper
{
	~VertexIndexWrapper() { SAFE_DELETE(myIndexData); };
	s8* myIndexData = nullptr;
	s32 myIndexCount = 0;
	s32 mySize = 0;
	DXGI_FORMAT myFormat;
};

struct IndexBufferWrapper
{
	~IndexBufferWrapper() { SAFE_RELEASE(myIndexBuffer); };
	ID3D11Buffer* myIndexBuffer = nullptr;
	DXGI_FORMAT myIndexBufferFormat;
	u16 myByteOffset = 0;
};
