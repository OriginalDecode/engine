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

		if (!CreateCommandBuffer())
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
		m_Instance = new VkInstance;
		VkResult result = vkCreateInstance(&instance_info, NULL, m_Instance);
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
		VkResult result = vkEnumeratePhysicalDevices(*m_Instance, &device_count, NULL);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to find valid devices for vulkan. VK_ERROR : %d", result);
			return false;
		}
		result = vkEnumeratePhysicalDevices(*m_Instance, &device_count, &in_out_device);
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
		m_PhysDevice = new VkPhysicalDevice;
		if (!EnumerateDevices(*m_PhysDevice))
			return false;

		u32 queue_family_count = 0;
		VkQueueFamilyProperties queue_properties;
		memset(&queue_properties, 0, sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(*m_PhysDevice, &queue_family_count, &queue_properties);

		VkDeviceQueueCreateInfo queue_info;
		memset(&queue_info, 0, sizeof(VkDeviceQueueCreateInfo));
		bool found = false;
		for (u32 i = 0; i < queue_family_count; i++)
		{
			if (queue_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queue_info.queueFamilyIndex = i;
				found = true;
				break;
			}
		}

		float queue_priorities[1] = { 0.0 };
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = NULL;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = queue_priorities;

		VkDeviceCreateInfo device_info;
		memset(&device_info, 0, sizeof(VkDeviceCreateInfo));
		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.pNext = NULL;
		device_info.queueCreateInfoCount = 1;
		device_info.pQueueCreateInfos = &queue_info;
		device_info.enabledExtensionCount = 0;
		device_info.ppEnabledExtensionNames = NULL;
		device_info.enabledLayerCount = 0;
		device_info.ppEnabledLayerNames = NULL;
		device_info.pEnabledFeatures = NULL;

		m_Device = new VkDevice;
		VkResult result = vkCreateDevice(*m_PhysDevice, &device_info, NULL, m_Device);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to create vulkanDevice. VK_ERROR : %d", result);
			return false;
		}

		if (!CreateCommandPool(queue_info.queueFamilyIndex))
			return false;

		return true;
	}

	bool Vulkan::CreateCommandPool(u32 queue_family_index)
	{

		m_CommandPool = new VkCommandPool;
		VkCommandPoolCreateInfo pool_info;
		memset(&pool_info, 0, sizeof(VkCommandPoolCreateInfo));
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.pNext = NULL;
		pool_info.queueFamilyIndex = queue_family_index;
		pool_info.flags = 0;
		VkResult result = vkCreateCommandPool(*m_Device, &pool_info, NULL, m_CommandPool);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to create VkCommandPool VK_ERROR : %d", result);
			return false;
		}
		return true;
	}

	bool Vulkan::CreateCommandBuffer()
	{
		VkCommandBufferAllocateInfo buffer_info;
		memset(&buffer_info, 0, sizeof(VkCommandBufferAllocateInfo));
		buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		buffer_info.pNext = NULL;
		buffer_info.commandPool = *m_CommandPool;
		buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		buffer_info.commandBufferCount = 1;
		m_CommandBuffer = new VkCommandBuffer;
		VkResult result = vkAllocateCommandBuffers(*m_Device, &buffer_info, &m_CommandBuffer[0]);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to allocated command buffers VK_ERROR : %d", result);
			return false;
		}
		return true;
	}

	bool Vulkan::CleanUp()
	{
		vkFreeCommandBuffers(*m_Device, *m_CommandPool, 1, m_CommandBuffer);
		vkDestroyCommandPool(*m_Device, *m_CommandPool, NULL);
		vkDestroyDevice(*m_Device, NULL);
		vkDestroyInstance(*m_Instance, NULL);

		SAFE_DELETE(m_Device);
		SAFE_DELETE(m_Instance);
		SAFE_DELETE(m_CommandPool);
		SAFE_DELETE(m_CommandBuffer);

		if (m_Device) return false;
		if (m_Instance) return false;
		if (m_CommandBuffer) return false;
		if (m_CommandPool) return false;

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