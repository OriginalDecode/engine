#include "stdafx.h"
#include "Vulkan.h"
#ifdef SNOWBLIND_VULKAN
namespace Snowblind
{
	bool Vulkan::Initiate(HWND window_handle, float window_width, float window_height)
	{
		myAPI = "Vulkan";
		myWindowHandle = window_handle;
		myWindowWidth = window_width;
		myWindowHeight = window_height;

		VkApplicationInfo app_info;
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pNext = NULL;
		app_info.pApplicationName = "Snowblind";
		app_info.applicationVersion = 1;
		app_info.pEngineName = "Snowblind - Engine";
		app_info.engineVersion = 1;
		app_info.apiVersion = VK_API_VERSION_1_0;
		
		VkInstanceCreateInfo instance_info;
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pNext = NULL;
		instance_info.flags = 0;
		instance_info.pApplicationInfo = &app_info;
		instance_info.enabledExtensionCount = 0;
		instance_info.ppEnabledExtensionNames = NULL;
		instance_info.enabledLayerCount = 0;
		instance_info.ppEnabledLayerNames = NULL;
		VkResult result = vkCreateInstance(&instance_info, NULL, &m_Instance);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("%d", result)
			return false;
		}
		return true;
	}

	bool Vulkan::CleanUp()
	{

		vkDestroyInstance(m_Instance, NULL);
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
#endif