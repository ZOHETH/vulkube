#include "utils.hpp"

#include <iostream>

namespace vk
{
	namespace su
	{
		WindowData::WindowData(GLFWwindow* wnd, std::string const& name, vk::Extent2D const& extent) : handle{wnd},
			name{name}, extent{extent}
		{
		}

		WindowData::WindowData(WindowData&& other) : handle{}, name{}, extent{}
		{
			std::swap(handle, other.handle);
			std::swap(name, other.name);
			std::swap(extent, other.extent);
		}

		WindowData::~WindowData() noexcept
		{
			glfwDestroyWindow(handle);
		}

		WindowData createWindow(std::string const& windowName, vk::Extent2D const& extent)
		{
			struct glfwContext
			{
				glfwContext()
				{
					glfwInit();
					glfwSetErrorCallback(
						[](int error, const char* msg)
						{
							std::cerr << "glfw: "
								<< "(" << error << ") " << msg << std::endl;
						});
				}

				~glfwContext()
				{
					glfwTerminate();
				}
			};

			static auto glfwCtx = glfwContext();
			(void)glfwCtx;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			GLFWwindow* window = glfwCreateWindow(extent.width, extent.height, windowName.c_str(), nullptr, nullptr);
			return WindowData(window, windowName, extent);
		}

		uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
		{
			const std::vector<vk::QueueFamilyProperties>::const_iterator graphicsQueueFamilyProperty =
				std::find_if(queueFamilyProperties.begin(),
				             queueFamilyProperties.end(),
				             [](vk::QueueFamilyProperties const& qfp)
				             {
					             return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
				             });
			assert(graphicsQueueFamilyProperty != queueFamilyProperties.end());
			return static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
		}

		std::vector<std::string> getInstanceExtensions()
		{
			std::vector<std::string> extensions;

			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			extensions.push_back("VK_KHR_win32_surface");
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
			extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_IOS_MVK )
			extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
			extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_MIR_KHR )
			extensions.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_VI_NN )
			extensions.push_back(VK_NN_VI_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
			extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_WIN32_KHR )
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XCB_KHR )
			extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
			extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
			extensions.push_back(VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME);
#endif
			return extensions;
		}


		vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT()
		{
			return {
				{},
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
				&vk::su::debugUtilsMessengerCallback
			};
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageTypes,
			VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
			void* /*pUserData*/)
		{
#if !defined( NDEBUG )
			if (pCallbackData->messageIdNumber == 648835635)
			{
				// UNASSIGNED-khronos-Validation-debug-build-warning-message
				return VK_FALSE;
			}
			if (pCallbackData->messageIdNumber == 767975156)
			{
				// UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
				return VK_FALSE;
			}
#endif
			std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
				<< vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
			std::cerr << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
			std::cerr << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
			std::cerr << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
			if (0 < pCallbackData->queueLabelCount)
			{
				std::cerr << std::string("\t") << "Queue Labels:\n";
				for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
				{
					std::cerr << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName <<
						">\n";
				}
			}
			if (0 < pCallbackData->cmdBufLabelCount)
			{
				std::cerr << std::string("\t") << "CommandBuffer Labels:\n";
				for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
				{
					std::cerr << std::string("\t\t") << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName <<
						">\n";
				}
			}
			if (0 < pCallbackData->objectCount)
			{
				std::cerr << std::string("\t") << "Objects:\n";
				for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
				{
					std::cerr << std::string("\t\t") << "Object " << i << "\n";
					std::cerr << std::string("\t\t\t") << "objectType   = " << vk::to_string(
							static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType))
						<< "\n";
					std::cerr << std::string("\t\t\t") << "objectHandle = " << pCallbackData->pObjects[i].objectHandle
						<< "\n";
					if (pCallbackData->pObjects[i].pObjectName)
					{
						std::cerr << std::string("\t\t\t") << "objectName   = <" << pCallbackData->pObjects[i].
							pObjectName << ">\n";
					}
				}
			}
			return VK_TRUE;
		}

		uint32_t findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits,
		                        vk::MemoryPropertyFlags requirementsMask)
		{
			auto typeIndex = static_cast<uint32_t>(~0);
			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
			{
				if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) ==
					requirementsMask))
				{
					typeIndex = i;
					break;
				}
				typeBits >>= 1;
			}
			assert(typeIndex != static_cast<uint32_t>(~0));
			return typeIndex;
		}


		std::vector<char const*> gatherLayers(std::vector<std::string> const& layers
#if !defined( NDEBUG )
		                                      ,
		                                      std::vector<vk::LayerProperties> const& layerProperties
#endif
		)
		{
			std::vector<char const*> enabledLayers;
			enabledLayers.reserve(layers.size());
			for (auto const& layer : layers)
			{
				assert(
					std::find_if(layerProperties.begin(), layerProperties.end(), [layer](vk::LayerProperties const& lp)
						{
						return layer == lp.layerName;
						}) != layerProperties.end());
			}
#if !defined( NDEBUG )
			if (std::find(layers.begin(), layers.end(), "VK_LAYER_KHRONOS_validation") == layers.end() &&
				std::find_if(layerProperties.begin(),
				             layerProperties.end(),
				             [](vk::LayerProperties const& lp)
				             {
					             return (strcmp("VK_LAYER_KHRONOS_validation", lp.layerName) == 0);
				             }) != layerProperties.end())
			{
				enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
			}
#endif
			return enabledLayers;
		}

		std::vector<char const*> gatherExtensions(std::vector<std::string> const& extensions
#if !defined( NDEBUG )
		                                          ,
		                                          std::vector<vk::ExtensionProperties> const& extensionProperties
#endif
		)
		{
			std::vector<char const*> enabledExtensions;
			enabledExtensions.reserve(extensions.size());
			for (auto const& ext : extensions)
			{
				assert(std::find_if(extensionProperties.begin(),
						extensionProperties.end(),
						[ext](vk::ExtensionProperties const& ep) { return ext == ep.extensionName; }) !=
					extensionProperties
					.end());
				enabledExtensions.push_back(ext.data());
			}
#if !defined( NDEBUG )
			if (std::find(extensions.begin(), extensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == extensions.end()
				&&
				std::find_if(extensionProperties.begin(),
				             extensionProperties.end(),
				             [](vk::ExtensionProperties const& ep)
				             {
					             return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0);
				             }) != extensionProperties.end())
			{
				enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
#endif
			return enabledExtensions;
		}

#if defined( NDEBUG )
		vk::StructureChain<vk::InstanceCreateInfo>
#else
		vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
			makeInstanceCreateInfoChain(vk::ApplicationInfo const& applicationInfo,
				std::vector<char const*> const& layers,
				std::vector<char const*> const& extensions)
		{
#if defined( NDEBUG )
			// in non-debug mode just use the InstanceCreateInfo for instance creation
			vk::StructureChain<vk::InstanceCreateInfo> instanceCreateInfo({ {}, &applicationInfo, layers, extensions });
#else
			// in debug mode, addionally use the debugUtilsMessengerCallback in instance creation!
			vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
			vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
			vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
				{ {}, &applicationInfo, layers, extensions }, { {}, severityFlags, messageTypeFlags, &vk::su::debugUtilsMessengerCallback });
#endif
			return instanceCreateInfo;
		}

		vk::PresentModeKHR pickPresentMode(std::vector<vk::PresentModeKHR> const& presentModes)
		{
			vk::PresentModeKHR pickedMode = vk::PresentModeKHR::eFifo;
			for (const auto& presentMode : presentModes)
			{
				if (presentMode == vk::PresentModeKHR::eMailbox)
				{
					pickedMode = presentMode;
					break;
				}

				if (presentMode == vk::PresentModeKHR::eImmediate)
				{
					pickedMode = presentMode;
				}
			}
			return pickedMode;
		}

		vk::SurfaceFormatKHR pickSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& formats)
		{
			assert(!formats.empty());
			vk::SurfaceFormatKHR pickedFormat = formats[0];
			if (formats.size() == 1)
			{
				if (formats[0].format == vk::Format::eUndefined)
				{
					pickedFormat.format = vk::Format::eB8G8R8A8Unorm;
					pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
				}
			}
			else
			{
				// request several formats, the first found will be used
				vk::Format requestedFormats[] = {
					vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm,
					vk::Format::eR8G8B8Unorm
				};
				vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
				for (auto requestedFormat : requestedFormats)
				{
					auto it = std::find_if(formats.begin(),
					                       formats.end(),
					                       [requestedFormat, requestedColorSpace](vk::SurfaceFormatKHR const& f)
					                       {
						                       return (f.format == requestedFormat) && (f.colorSpace ==
							                       requestedColorSpace);
					                       });
					if (it != formats.end())
					{
						pickedFormat = *it;
						break;
					}
				}
			}
			assert(pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);
			return pickedFormat;
		}
	}
}
