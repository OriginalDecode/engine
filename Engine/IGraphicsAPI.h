#pragma once
#include "snowblind_shared.h"
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
	std::string m_APIName;
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
	DEPTH_TEST,
	_COUNT
};

enum class eRasterizer
{
	WIREFRAME,
	CULL_BACK,
	CULL_NONE,
	CULL_FRONT,
	MSAA,
	_COUNT
};

enum class eBlendStates
{
	NO_BLEND,
	LIGHT_BLEND,
	ALPHA_BLEND,
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
		std::string GetAPIName() { return m_CreateInfo.m_APIName; }

		virtual void EnableZBuffer() = 0;
		virtual void DisableZBuffer() = 0;
		virtual IDevice* GetDevice() = 0;



	protected:
		CreateInfo m_CreateInfo;

		std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
	};

};