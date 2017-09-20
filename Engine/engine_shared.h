#pragma once
#include <standard_datatype.hpp>
#include <CommonLib/HashString.h>

#include "EngineDefines.h"
#include <Engine/RenderContext.h>
#include <profile_defines.h>

#include <Windows.h>

#define ARRSIZE(x) sizeof(x) / sizeof(x[0])

#define BITFLAG(x) (1 << x)

typedef s32 Entity;

typedef void ITexture2D;
typedef void ITexture3D;
typedef void IShaderResourceView;
typedef void IDepthStencilView;
typedef void IRenderTargetView;
typedef void IViewport;
typedef void IVertexShader;
typedef void IPixelShader;
typedef void IGeometryShader;
typedef void IHullShader;
typedef void IDomainShader;
typedef void IComputeShader;
typedef void IShaderBlob;
typedef void IBuffer;
typedef void IInputLayout;
typedef void ISamplerState;
typedef void IDepthStencilState;
typedef void IRasterizerState;
typedef void IBlendState;

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
};

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
};

namespace clearcolor
{
	constexpr float black[4] = { 0.f, 0.f, 0.f, 0.f };
	constexpr float white[4] = { 1.f, 1.f, 1.f, 0.f };
	constexpr float pink[4] = { 1.f, 0.51f, 0.96f, 0.f };
};

namespace blendcolor
{
	constexpr float black[4] = { 0.f, 0.f, 0.f, 0.f };
};