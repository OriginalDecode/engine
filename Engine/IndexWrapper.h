#pragma once
enum DXGI_FORMAT;
struct ID3D11Buffer;

namespace Snowblind
{
	struct SVertexIndexWrapper
	{
		~SVertexIndexWrapper(){ delete myIndexData; myIndexData = nullptr; };
		char* myIndexData;
		int myIndexCount;
		int mySize;
		DXGI_FORMAT myFormat;
	};

	struct SIndexBufferWrapper
	{
		~SIndexBufferWrapper(){ myIndexBuffer->Release(); myIndexBuffer = nullptr; };
		ID3D11Buffer* myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
		int myByteOffset;
	};
};