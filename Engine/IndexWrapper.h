#pragma once
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
enum DXGI_FORMAT;
struct ID3D11Buffer;

struct IndexDataWrapper
{
	~IndexDataWrapper() { SAFE_DELETE(myIndexData); };
	s8* myIndexData = nullptr;
	s32 myIndexCount = 0;
	s32 mySize = 0;
	graphics::VertexFormat m_Format;
};

struct IndexBufferWrapper
{
	~IndexBufferWrapper() 
	{ 
		/*SAFE_RELEASE(myIndexBuffer);*/ 
	};
	IBuffer* myIndexBuffer = nullptr;
	graphics::VertexFormat myIndexBufferFormat;
	u16 myByteOffset = 0;
};
