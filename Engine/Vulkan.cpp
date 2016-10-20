#include "stdafx.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include "Vulkan.h"
#ifdef SNOWBLIND_VULKAN
namespace Snowblind
{
	bool Vulkan::Initiate(CreateInfo create_info)
	{
		m_CreateInfo = create_info;
		if (!CreateVKInstance())
			return false;

		if (!EnumerateDevices())
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

		std::vector<char*> extension;
		extension.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extension.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#ifdef VALIDATE_VULKAN
		extension.push_back("VK_EXT_debug_report");
#endif

		std::vector<char*> layers;
#ifdef VALIDATE_VULKAN
		layers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif


		VkInstanceCreateInfo instance_info;
		memset(&instance_info, 0, sizeof(VkInstanceCreateInfo));
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pNext = NULL;
		instance_info.flags = 0;
		instance_info.pApplicationInfo = &app_info;
		instance_info.enabledExtensionCount = u32(extension.size());
		instance_info.ppEnabledExtensionNames = extension.data();
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

	bool Vulkan::EnumerateDevices()
	{

		u32 device_count = 0;
		VkResult result = vkEnumeratePhysicalDevices(*m_Instance, &device_count, NULL);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to find valid devices for vulkan. VK_ERROR : %d", result);
			return false;
		}

		m_PhysDevice = new VkPhysicalDevice;
		result = vkEnumeratePhysicalDevices(*m_Instance, &device_count, m_PhysDevice);
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
		u32 queue_family_count = 0;
		VkQueueFamilyProperties queue_properties;
		memset(&queue_properties, 0, sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(*m_PhysDevice, &queue_family_count, &queue_properties);

		VkDeviceQueueCreateInfo queue_info;
		memset(&queue_info, 0, sizeof(VkDeviceQueueCreateInfo));
		for (u32 i = 0; i < queue_family_count; i++)
		{
			if (queue_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queue_info.queueFamilyIndex = i;
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


		SwapchainCreateInfo sc_create_info;
		memset(&sc_create_info, 0, sizeof(SwapchainCreateInfo));


		if (!CreateSurface(queue_info.queueFamilyIndex, queue_family_count, sc_create_info))
			return false;


		if (!CreateSwapchain(sc_create_info))
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

	bool Vulkan::CreateSwapchain(SwapchainCreateInfo& swapchain_create_info)
	{

		if (!GetPresentMode(swapchain_create_info))
			return false;

		VkSwapchainCreateInfoKHR create_info;
		memset(&create_info, 0, sizeof(VkSwapchainCreateInfoKHR));
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.pNext = NULL;
		create_info.surface = *m_Surface;
		create_info.imageFormat = m_Format;
		create_info.minImageCount = swapchain_create_info.swapchain_image_count;
		create_info.imageExtent.width = swapchain_create_info.swapchain_extent.width;
		create_info.imageExtent.height = swapchain_create_info.swapchain_extent.height;
		create_info.preTransform = swapchain_create_info.surface_transform_out;
		create_info.presentMode = swapchain_create_info.present_mode_out;
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.imageArrayLayers = 1;
		create_info.oldSwapchain = VK_NULL_HANDLE;
		create_info.clipped = true;
		create_info.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = NULL;

		m_Swapchain = new VkSwapchainKHR;
		VkResult result = vkCreateSwapchainKHR(*m_Device, &create_info, NULL, m_Swapchain);

		return true;
	}

	bool Vulkan::CreateSurface(u32 queue_family_index, u32 queue_family_count_in, SwapchainCreateInfo& swapchain_create_info)
	{
		VkWin32SurfaceCreateInfoKHR create_info;
		create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		create_info.pNext = NULL;
		create_info.hinstance = m_CreateInfo.m_Instance;
		create_info.hwnd = m_CreateInfo.m_HWND;

		m_Surface = new VkSurfaceKHR;
		VkResult result = vkCreateWin32SurfaceKHR(*m_Instance, &create_info, NULL, m_Surface);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to create a surface for vulkan. VK_ERROR : %d", result);
			return false;
		}

		return true;
	}

	bool Vulkan::GetPresentMode(SwapchainCreateInfo& swapchain_create_info)
	{
		VkSurfaceCapabilitiesKHR capabilities;
		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*m_PhysDevice, *m_Surface, &capabilities);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to find any surface capabilities on graphics card. VK_ERROR : %d", result);
			return false;
		}

		u32 present_mode_count = 0;
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(*m_PhysDevice, *m_Surface, &present_mode_count, NULL);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to get any present modes from graphics card. VK_ERROR : %d", result);
			return false;
		}

		if (present_mode_count == 0)
		{
			DL_MESSAGE("Present mode count was %d, VK_ERROR : %d", present_mode_count, result);
			return false;
		}

		VkPresentModeKHR* present_modes = (VkPresentModeKHR*)malloc(present_mode_count * sizeof(VkPresentModeKHR));
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(*m_PhysDevice, *m_Surface, &present_mode_count, present_modes);
		if (result != VK_SUCCESS)
		{
			DL_MESSAGE("Failed to get any present modes from graphics card. VK_ERROR : %d", result);
			return false;
		}

		if (capabilities.currentExtent.width == 0xFFFFFFFF)
		{
			swapchain_create_info.swapchain_extent.width = m_CreateInfo.m_WindowWidth;
			swapchain_create_info.swapchain_extent.height = m_CreateInfo.m_WindowHeight;

			if (swapchain_create_info.swapchain_extent.width < capabilities.minImageExtent.width)
			{
				swapchain_create_info.swapchain_extent.width = capabilities.minImageExtent.width;
			}
			else if (swapchain_create_info.swapchain_extent.width > capabilities.maxImageExtent.width)
			{
				swapchain_create_info.swapchain_extent.width = capabilities.maxImageExtent.width;
			}

			if (swapchain_create_info.swapchain_extent.height < capabilities.minImageExtent.height)
			{
				swapchain_create_info.swapchain_extent.height = capabilities.minImageExtent.height;
			}
			else if (swapchain_create_info.swapchain_extent.height > capabilities.maxImageExtent.height)
			{
				swapchain_create_info.swapchain_extent.height = capabilities.maxImageExtent.height;
			}
		}
		else
		{
			swapchain_create_info.swapchain_extent = capabilities.currentExtent;
		}

		for (u32 i = 0; i < present_mode_count; i++)
		{
			if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				swapchain_create_info.present_mode_out = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if ((swapchain_create_info.present_mode_out != VK_PRESENT_MODE_MAILBOX_KHR) && (present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
			{
				swapchain_create_info.present_mode_out = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}

		swapchain_create_info.swapchain_image_count = capabilities.minImageCount;
		if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			swapchain_create_info.surface_transform_out = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			swapchain_create_info.surface_transform_out = capabilities.currentTransform;
		}


		return true;
	}

	bool Vulkan::CleanUp()
	{
		vkDestroySwapchainKHR(*m_Device, *m_Swapchain, NULL);
		vkDestroySurfaceKHR(*m_Instance, *m_Surface, NULL);
		vkFreeCommandBuffers(*m_Device, *m_CommandPool, 1, m_CommandBuffer);
		vkDestroyCommandPool(*m_Device, *m_CommandPool, NULL);
		vkDestroyDevice(*m_Device, NULL);
		vkDestroyInstance(*m_Instance, NULL);

		SAFE_DELETE(m_Swapchain);
		SAFE_DELETE(m_Device);
		SAFE_DELETE(m_Instance);
		SAFE_DELETE(m_CommandPool);
		SAFE_DELETE(m_CommandBuffer);
		SAFE_DELETE(m_Surface);

		if (m_Device) return false;
		if (m_Instance) return false;
		if (m_CommandBuffer) return false;
		if (m_CommandPool) return false;
		if (m_Surface) return false;
		if (m_Swapchain) return false;

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