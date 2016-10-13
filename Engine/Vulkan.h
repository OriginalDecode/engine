#pragma once
#include "IGraphicsAPI.h"

#ifdef SNOWBLIND_VULKAN
#include <vulkan/vulkan.h>

namespace Snowblind
{
	class Vulkan : public IGraphicsAPI
	{
	public:
		Vulkan() = default;
		//__________________________
		// Virtual Functions

		bool Initiate(HWND window_handle, float window_width, float window_height) override;


		bool CleanUp() override;

		void Present(u8 anInterval, u8 flags) override;
		void Clear() override;

		void OnAltEnter() override;

	private:
		bool CreateVKInstance();
		bool EnumerateDevices(VkPhysicalDevice& in_out_device);
		bool CreateDevice();
		bool CreateCommandPool(u32 queue_family_index);
		bool CreateCommandBuffer();
		//Too much stuff?
		
		VkDevice* m_Device;
		VkCommandBuffer* m_CommandBuffer;
		VkCommandPool* m_CommandPool;
		VkInstance* m_Instance;
		VkPhysicalDevice* m_PhysDevice;

	};
};
#endif