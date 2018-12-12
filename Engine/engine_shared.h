#pragma once
#include <standard_datatype.hpp>

#include "EngineDefines.h"
#include <Engine/RenderContext.h>
#include "profile_defines.h"

#include <Windows.h>

#include "../CommonLib/Math/Vector/Vector.h"


#define ARRSIZE(x) sizeof(x) / sizeof(x[0])

#define BIT(x) (1 << x)

#define STRINGIFY(_class) #_class

#define DEBUG_NAME(name,_class) name  #_class
#define DEBUG_NAME_A(str,_class) str + #_class

#define CLASS_NAME_STR(_class) #_class



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


typedef uint32 Entity;

typedef void ITexture2D;
typedef void ITexture3D;
typedef void IShaderResourceView;
typedef void IDepthStencilView;
typedef void IRenderTargetView;
typedef void IViewport;
typedef void IVertexShader;
typedef void IFragmentShader;
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
typedef uint64 uptr;
#else
typedef uint32 uptr;
#endif

enum eShaderType
{
	VERTEX,
	PIXEL,
	GEOMETRY,
	HULL,
	DOMAINS,
	COMPUTE,
	NOF_TYPES
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


struct HashType
{
	union
	{
		uint64 m_Hash = 0;
		struct {
			uint32 m_Lower;
			uint32 m_Upper;
		};
	};

	HashType() = default;

	HashType(const HashType& other)
	{
		m_Hash = other.m_Hash;
	}

	HashType(const uint64& other)
	{
		m_Hash = other;
	}

	bool operator<(const HashType& other) const
	{
		return (m_Lower < other.m_Lower);
	}

	HashType& operator=(const uint64& lhv)
	{
		m_Hash = lhv;
	}

};

