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

		if (!CreateVKInstance())
			return false;

		if (!CreateDevice())
			return false;

		return true;
	}

	bool Vulkan::CreateVKInstance()
	{
		VkApplicationInfo app_info;
		memset(&app_info, 0, sizeof(VkApplicationInfo));
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pNext = NULL;
		app_info.pApplicationName = "Snowblind";
		app_info.applicationVersion = 1;
		app_info.pEngineName = "Snowblind - Engine";
		app_info.engineVersion = 1;
		app_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instance_info;
		memset(&instance_info, 0, sizeof(VkInstanceCreateInfo));
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
			DL_MESSAGE("Failed to create VkInstance VK_ERROR : %d", result)
				return false;
		}

		DL_MESSAGE("Successfully created VkInstance");
		return true;
	}

	bool Vulkan::EnumerateDevices(VkPhysicalDevice& in_out_device)
	{
		u32 device_count = 0;
		VkResult result = vkEnumeratePhysicalDevices(m_Instance, &device_count, NULL);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to find valid devices for vulkan. VK_ERROR : %d", result);
			return false;
		}
		result = vkEnumeratePhysicalDevices(m_Instance, &device_count, &in_out_device);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to enumerate selected device. VK_ERROR : %d", result);
			return false;
		}

		DL_MESSAGE("Successfully enumerated devices.\nDevice Count : %d", device_count);
		return true;
	}

	bool Vulkan::CreateDevice()
	{
		VkPhysicalDevice device;
		if (!EnumerateDevices(device))
			return false;


		VkQueueFamilyProperties queue_properties;
		memset(&queue_properties, 0, sizeof(VkQueueFamilyProperties));

		u32 queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, &queue_properties);

		bool found = false;
		for (unsigned int i = 0; i < queue_family_count; i++) 
		{
			if (queue_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queue_info.queueFamilyIndex = i;
				found = true;
				break;
			}
		}

		//float queue_priorities[1] = { 0.0 };
		//queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		//queue_info.pNext = NULL;
		//queue_info.queueCount = 1;
		//queue_info.pQueuePriorities = queue_priorities;*/



		VkDeviceCreateInfo device_info;
		memset(&device_info, 0, sizeof(VkDeviceCreateInfo));
		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.pNext = NULL;
		device_info.queueCreateInfoCount = 1;
		//device_info.pQueueCreateInfos = &queue_info; //What?
		device_info.enabledExtensionCount = 0;
		device_info.ppEnabledExtensionNames = NULL;
		device_info.enabledLayerCount = 0;
		device_info.ppEnabledLayerNames = NULL;
		device_info.pEnabledFeatures = NULL;

		VkResult result = vkCreateDevice(device, &device_info, NULL, &m_Device);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to create vulkanDevice. VK_ERROR : %d", result);
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