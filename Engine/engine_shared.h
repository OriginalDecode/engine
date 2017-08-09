#pragma once
#include <Windows.h>
#include <standard_datatype.hpp>
#include "EngineDefines.h"
#include <DL_Debug/DL_Debug.h>
#include <string>

#ifdef _PROFILE
#include <easy/profiler.h>
#include <easy/reader.h>
#endif


#define BITFLAG(x) (1 << x)

typedef s32 Entity;

/**
typedef void* ITexture2D;
typedef void* ITexture3D;
typedef void* IShaderResourceView;
typedef void* IDepthStencilView;
typedef void* IRenderTargetView;
typedef void* IViewport;
typedef void* IVertexShader;
typedef void* IPixelShader;
typedef void* IGeometryShader;
typedef void* IHullShader;
typedef void* IDomainShader;
typedef void* IComputeShader;
typedef void* IShaderBlob;
typedef void* IDevice;
typedef void* IDeviceCtx;
typedef void* IBuffer;
typedef void* IInputLayout;
**/




typedef struct ID3D11Texture2D				ITexture2D;
typedef struct ID3D11ShaderResourceView		IShaderResourceView;
typedef struct ID3D11DepthStencilView		IDepthStencilView;
typedef struct ID3D11RenderTargetView		IRenderTargetView;

typedef enum DXGI_FORMAT					TextureFormat;
typedef enum D3D11_USAGE					UsageType;
typedef struct D3D11_VIEWPORT				Viewport;
typedef struct ID3D11VertexShader			IVertexShader;
typedef struct ID3D11PixelShader			IPixelShader;
typedef struct ID3D11GeometryShader			IGeometryShader;
typedef struct ID3D11HullShader				IHullShader;
typedef struct ID3D11DomainShader			IDomainShader;
typedef struct ID3D11ComputeShader			IComputeShader;
typedef struct ID3D10Blob					IBlob;

typedef struct ID3D11Device					IDevice;
typedef struct ID3D11DeviceContext			IDevContext;

typedef struct ID3D11Buffer					IBuffer;
typedef struct ID3D11InputLayout			IInputLayout;


#if (UINTPTR_MAX == 0xffffffffffffffff)
typedef u64 uptr;
#else
typedef u32 uptr;
#endif

enum class eShaderType : s32
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

struct RenderContext
{
	class Engine* m_Engine = nullptr;
	struct ID3D11Device* m_Device = nullptr;
	struct ID3D11DeviceContext* m_Context = nullptr;
	class DirectX11* m_API = nullptr;
};