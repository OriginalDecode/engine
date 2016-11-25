#pragma once
#include <Windows.h>
#include <standard_datatype.hpp>
#ifdef SNOWBLIND_DX11
typedef struct ID3D11Texture2D				ITexture2D;
typedef struct ID3D11ShaderResourceView		IShaderResourceView;
typedef struct ID3D11DepthStencilView		IDepthStencilView;
typedef struct ID3D11RenderTargetView		IRenderTargetView;

typedef DXGI_FORMAT							TextureFormat;
typedef D3D11_USAGE							UsageType;

typedef struct ID3D11VertexShader			IVertexShader;
typedef struct ID3D11PixelShader			IPixelShader;
typedef struct ID3D11GeometryShader			IGeometryShader;
typedef struct ID3D11HullShader				IHullShader;
typedef struct ID3D11DomainShader			IDomainShader;
typedef struct ID3D11ComputeShader			IComputeShader;
typedef struct ID3D10Blob					IBlob;

typedef struct ID3D11Device					IDevice;
typedef struct ID3D11DeviceContext			IContext_;

#else

#endif

enum class eShaderType
{
	VERTEX,
	PIXEL,
	GEOMETRY,
	HULL,
	DOMAINS,
	COMPUTE,
	_COUNT
};




struct Ticket_Mutex
{
	u64 volatile ticket = 0;
	u64 volatile serving = 0;
};

inline u64 AtmoicAddU64(u64 volatile *value, u64 toAdd)
{
	u64 result = _InterlockedExchange64((__int64 volatile *)value, (*value + toAdd));
	return result;
};

inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	u64 ticket = AtmoicAddU64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving)
	{
	
	}
}

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
}

