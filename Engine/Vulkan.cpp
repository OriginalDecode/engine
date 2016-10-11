#include "stdafx.h"
#include "Vulkan.h"

namespace Snowblind
{

	bool Vulkan::Initiate(HWND window_handle, float window_width, float window_height)
	{
		myAPI = "Vulkan";
		myWindowHandle = window_handle;
		myWindowWidth = window_width;
		myWindowHeight = window_height;

		return true;
	}

	bool Vulkan::CleanUp()
	{
		return true;
	}

	void Vulkan::Present(u8 anInterval, u8 flags)
	{

	}

	void Vulkan::Clear()
	{

	}

	void Vulkan::OnAltEnter()
	{

	}

};