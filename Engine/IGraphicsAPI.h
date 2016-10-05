#pragma once
#include "standard_datatype.hpp"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

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
	class CIGraphicsAPI
	{
	public:
		virtual bool Initiate(HWND window_handler, float window_width, float window_height) = 0;
		virtual bool CleanUp() = 0;

		virtual void Clear() = 0;
		virtual void Present(u8 refresh_rate, u8 flags) = 0;

		virtual void OnAltEnter() = 0;

	protected:
		HWND myWindowHandle;
		float myWindowWidth = 0.f;
		float myWindowHeight = 0.f;
		const char* myAPI = { '\0' };

		std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
	};

};
