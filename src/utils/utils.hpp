#pragma once
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace vk
{
	namespace su
	{
		const uint64_t FenceTimeout = 100000000;

		template <class T>
		VULKAN_HPP_INLINE constexpr const T& clamp(const T& v, const T& lo, const T& hi)
		{
			return v < lo ? lo : hi < v ? hi : v;
		}

		struct WindowData
		{
			WindowData(GLFWwindow* wnd, std::string const& name, vk::Extent2D const& extent);
			WindowData(const WindowData&) = delete;
			WindowData(WindowData&& other);
			~WindowData() noexcept;

			GLFWwindow* handle;
			std::string name;
			vk::Extent2D extent;
		};

		WindowData createWindow(std::string const& windowName, vk::Extent2D const& extent);

		uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);

		std::vector<std::string> getInstanceExtensions();

		inline std::vector<std::string> getDeviceExtensions()
		{
			return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		}

		template <typename TargetType, typename SourceType>
		VULKAN_HPP_INLINE TargetType checked_cast(SourceType value)
		{
			static_assert(sizeof(TargetType) <= sizeof(SourceType), "No need to cast from smaller to larger type!");
			static_assert(std::numeric_limits<SourceType>::is_integer, "Only integer types supported!");
			static_assert(!std::numeric_limits<SourceType>::is_signed, "Only unsigned types supported!");
			static_assert(std::numeric_limits<TargetType>::is_integer, "Only integer types supported!");
			static_assert(!std::numeric_limits<TargetType>::is_signed, "Only unsigned types supported!");
			assert(value <= std::numeric_limits<TargetType>::max());
			return static_cast<TargetType>(value);
		}

		vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT();
		VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageTypes,
			VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
			void* /*pUserData*/);

		uint32_t findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits,
		                        vk::MemoryPropertyFlags requirementsMask);
		std::vector<char const*> gatherLayers(std::vector<std::string> const& layers
#if !defined( NDEBUG )
		                                      ,
		                                      std::vector<vk::LayerProperties> const& layerProperties
#endif
		);
		std::vector<char const*> gatherExtensions(std::vector<std::string> const& extensions
#if !defined( NDEBUG )
		                                          ,
		                                          std::vector<vk::ExtensionProperties> const& extensionProperties
#endif
		);
#if defined( NDEBUG )
		vk::StructureChain<vk::InstanceCreateInfo>
#else
		vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
		makeInstanceCreateInfoChain(vk::ApplicationInfo const& applicationInfo,
		                            std::vector<char const*> const& layers,
		                            std::vector<char const*> const& extensions);

		vk::Format pickDepthFormat(vk::PhysicalDevice const& physicalDevice);
		vk::PresentModeKHR pickPresentMode(std::vector<vk::PresentModeKHR> const& presentModes);
		vk::SurfaceFormatKHR pickSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& formats);
	}
}
