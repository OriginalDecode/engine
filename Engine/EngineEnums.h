#pragma once
namespace Snowblind
{
	enum eAPIFlag
	{
#if defined(_WIN32) || defined(_WIN64)
		DIRECTX11,
		DIRECTX12,
#endif
		OPENGL,
		VULKAN,
		_COUNT
	};
};