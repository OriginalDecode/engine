#pragma once
#include <standard_datatype.hpp>

#include "EngineDefines.h"
#include <Engine/RenderContext.h>
#include "profile_defines.h"

#include <Windows.h>

#include "../CommonLib/Math/Vector/Vector.h"

#define ARRSIZE(x) sizeof(x) / sizeof(x[0])

#define BITFLAG(x) (1 << x)

#define STRINGIFY(_class) #_class

#define DEBUG_NAME(name,_class) name  #_class
#define DEBUG_NAME_A(str,_class) str + #_class
struct LinePoint
{
	LinePoint(const CU::Vector4f& pos, const CU::Vector4f& col)
		: position(pos)
		, color(col)
	{

	}
	LinePoint() { color = { 255.f, 255.f, 255.f, 255.f }; }
	CU::Vector4f position;
	CU::Vector4f color;
};

struct Line
{
	Line() {}
	Line(CU::Vector4f start, CU::Vector4f end)
	{
		m_Points[0].position = start;
		m_Points[0].color = { 255.f, 255.f , 255.f , 255.f };
		m_Points[1].position = end;
		m_Points[1].color = { 255.f, 255.f , 255.f , 255.f };
	}
	Line(LinePoint first, LinePoint second)
	{
		m_Points[0] = first;
		m_Points[1] = second;
	}
	LinePoint m_Points[2];
};


typedef u32 Entity;

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

inline u64 AtomicAddU64(u64 volatile *value, u64 toAdd)
{
	u64 _return = _InterlockedExchange64((__int64 volatile *)value, (*value + toAdd));
	return _return;
};

inline u32 AtomicAddU32(u32 volatile *value, u32 toAdd)
{
	u32 _return = InterlockedExchange((unsigned __int32 volatile *)value, (*value + toAdd));
	return _return;
};

inline s32 AtomicAddS32(s32 volatile *value, s32 toAdd)
{
	s32 _return = InterlockedExchange((unsigned __int32 volatile *)value, (*value + toAdd));
	return _return;
};

inline void* AtomicAddPtr(void* volatile* pointer, u64 size)
{
	void* pAddr = (void*)((u64)pointer + size);
	void * out = InterlockedExchangePointer(pointer, pAddr);
	return out;
}


inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	u64 ticket = AtomicAddU64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving)
	{
		//should have a timer or something here in debug to auto crash
	}

};

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
};

namespace clearcolor
{
	constexpr float black[4] = { 0.f, 0.f, 0.f, 0.f };
	constexpr float blue[4] = { 0.29f, 0.27f, 1.f,0.f };
	constexpr float white[4] = { 1.f, 1.f, 1.f, 0.f };
	constexpr float pink[4] = { 1.f, 0.51f, 0.96f, 0.f };
};

namespace blendcolor
{
	constexpr float black[4] = { 0.f, 0.f, 0.f, 0.f };
};



