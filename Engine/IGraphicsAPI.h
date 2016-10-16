#pragma once
#include "standard_datatype.hpp"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

struct CreateInfo
{
	HWND m_HWND;
	HINSTANCE m_Instance;
	float m_WindowWidth = 0.f;
	float m_WindowHeight = 0.f;
	const char* m_APIName = '\0';
};



enum class eEngineFlags
{
	FULLSCREEN,
	_COUNT
};

enum class eDepthStencil
{
	Z_ENABLED,
	Z_DISABLED,
	READ_NO_WRITE,
	MASK_TEST,
	LIGHT_MASK,
	_COUNT
};

enum class eRasterizer
{
	WIREFRAME,
	CULL_BACK,
	CULL_NONE,
	_COUNT
};

enum class eBlendStates
{
	NO_BLEND,
	LIGHT_BLEND,
	ALPHA_BLEND,
	PARTICLE_BLEND,
	BLEND_FALSE,
	_COUNT
};

enum class eSamplerStates
{
	LINEAR_CLAMP,
	LINEAR_WRAP,
	POINT_CLAMP,
	POINT_WRAP,
	NONE,
	_COUNT
};

namespace Snowblind
{
	class IGraphicsAPI
	{
	public:
		virtual bool Initiate(CreateInfo create_info) = 0;
		virtual bool CleanUp() = 0;

		virtual void Clear() = 0;
		virtual void Present(u8 refresh_rate, u8 flags) = 0;

		virtual void OnAltEnter() = 0;
		const std::string& GetAPIName() { return m_CreateInfo.m_APIName; }
	protected:
		CreateInfo m_CreateInfo;

		std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
	};

};