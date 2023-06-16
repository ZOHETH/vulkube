#if defined( _MSC_VER )
// no need to ignore any warnings with MSVC
#elif defined( __GNUC__ )
#  if ( 9 <= __GNUC__ )
#    pragma GCC diagnostic ignored "-Winit-list-lifetime"
#  endif
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif


#include "utils/geometries.hpp"
#include "utils/math.hpp"
#include "utils/shaders.hpp"
#include "utils/utils.hpp"
#include "utils/raii.hpp"


#include <iostream>
#include <thread>

static char const* AppName = "DrawCube";
static char const* EngineName = "Vulkan.hpp";

int main(int /*argc*/, char** /*argv*/)
{
	try
	{
		vk::raii::Context context;
		vk::raii::Instance instance = vk::raii::su::makeInstance(context, AppName, EngineName, {},
		                                                         vk::su::getInstanceExtensions());
#if !defined( NDEBUG )
		vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger(instance, vk::su::makeDebugUtilsMessengerCreateInfoEXT());
#endif
		vk::raii::PhysicalDevice physicalDevice = vk::raii::PhysicalDevices(instance).front();

		vk::raii::su::SurfaceData surfaceData(instance, AppName, vk::Extent2D(500, 500));

		std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex =
			vk::raii::su::findGraphicsAndPresentQueueFamilyIndex(physicalDevice, surfaceData.surface);
		vk::raii::Device device = vk::raii::su::makeDevice(physicalDevice, graphicsAndPresentQueueFamilyIndex.first,
		                                                   vk::su::getDeviceExtensions());

		vk::raii::CommandPool commandPool =
			vk::raii::CommandPool(device, {
				                      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
				                      graphicsAndPresentQueueFamilyIndex.first
			                      });
		vk::raii::CommandBuffer commandBuffer = vk::raii::su::makeCommandBuffer(device, commandPool);

		vk::raii::Queue graphicsQueue(device, graphicsAndPresentQueueFamilyIndex.first, 0);
		vk::raii::Queue presentQueue(device, graphicsAndPresentQueueFamilyIndex.second, 0);

		vk::raii::su::SwapChainData swapChainData(physicalDevice,
		                                          device,
		                                          surfaceData.surface,
		                                          surfaceData.extent,
		                                          vk::ImageUsageFlagBits::eColorAttachment |
		                                          vk::ImageUsageFlagBits::eTransferSrc,
		                                          {},
		                                          graphicsAndPresentQueueFamilyIndex.first,
		                                          graphicsAndPresentQueueFamilyIndex.second);

		vk::raii::su::DepthBufferData
			depthBufferData(physicalDevice, device, vk::Format::eD16Unorm, surfaceData.extent);

		vk::raii::su::BufferData uniformBufferData(physicalDevice, device, sizeof(glm::mat4x4),
		                                           vk::BufferUsageFlagBits::eUniformBuffer);
		glm::mat4x4 mvpcMatrix = vk::su::createModelViewProjectionClipMatrix(surfaceData.extent);
		vk::raii::su::copyToDevice(uniformBufferData.deviceMemory, mvpcMatrix);

		vk::raii::DescriptorSetLayout descriptorSetLayout =
			vk::raii::su::makeDescriptorSetLayout(device, {
				                                      {
					                                      vk::DescriptorType::eUniformBuffer, 1,
					                                      vk::ShaderStageFlagBits::eVertex
				                                      }
			                                      });
		vk::raii::PipelineLayout pipelineLayout(device, {{}, *descriptorSetLayout});

		vk::Format colorFormat = vk::su::pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(*surfaceData.surface)).
			format;
		vk::raii::RenderPass renderPass = vk::raii::su::makeRenderPass(device, colorFormat, depthBufferData.format);

		vk::raii::ShaderModule vertexShaderModule = vk::raii::su::makeShaderModule(
			device, "src/shaders/vert.spv");
		vk::raii::ShaderModule fragmentShaderModule = vk::raii::su::makeShaderModule(
			device, "src/shaders/frag.spv");

		std::vector<vk::raii::Framebuffer> framebuffers =
			vk::raii::su::makeFramebuffers(device, renderPass, swapChainData.imageViews, &depthBufferData.imageView,
			                               surfaceData.extent);

		vk::raii::su::BufferData vertexBufferData(physicalDevice, device, sizeof(coloredCubeData),
		                                          vk::BufferUsageFlagBits::eVertexBuffer);
		vk::raii::su::copyToDevice(vertexBufferData.deviceMemory, coloredCubeData,
		                           std::size(coloredCubeData));

		vk::raii::DescriptorPool descriptorPool = vk::raii::su::makeDescriptorPool(
			device, {{vk::DescriptorType::eUniformBuffer, 1}});
		vk::raii::DescriptorSet descriptorSet = std::move(
			vk::raii::DescriptorSets(device, {*descriptorPool, *descriptorSetLayout}).front());
		vk::raii::su::updateDescriptorSets(
			device, descriptorSet, {
				{vk::DescriptorType::eUniformBuffer, uniformBufferData.buffer, VK_WHOLE_SIZE, nullptr}
			}, {});

		vk::raii::PipelineCache pipelineCache(device, vk::PipelineCacheCreateInfo());
		vk::raii::Pipeline graphicsPipeline =
			vk::raii::su::makeGraphicsPipeline(device,
			                                   pipelineCache,
			                                   vertexShaderModule,
			                                   nullptr,
			                                   fragmentShaderModule,
			                                   nullptr,
			                                   vk::su::checked_cast<uint32_t>(sizeof(coloredCubeData[0])),
			                                   {
				                                   {vk::Format::eR32G32B32A32Sfloat, 0},
				                                   {vk::Format::eR32G32B32A32Sfloat, 16}
			                                   },
			                                   vk::FrontFace::eClockwise,
			                                   true,
			                                   pipelineLayout,
			                                   renderPass);

		/* VULKAN_KEY_START */
		while (!surfaceData.shouldClose())
		{
			glfwPollEvents();
			// Get the index of the next available swapchain image:
			vk::raii::Semaphore imageAcquiredSemaphore(device, vk::SemaphoreCreateInfo());

			vk::Result result;
			uint32_t imageIndex;
			std::tie(result, imageIndex) = swapChainData.swapChain.acquireNextImage(
				vk::su::FenceTimeout, *imageAcquiredSemaphore);
			assert(result == vk::Result::eSuccess);
			assert(imageIndex < swapChainData.images.size());

			commandBuffer.begin({});

			std::array<vk::ClearValue, 2> clearValues;
			clearValues[0].color = vk::ClearColorValue(0.2f, 0.2f, 0.2f, 0.2f);
			clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
			vk::RenderPassBeginInfo renderPassBeginInfo(*renderPass, *framebuffers[imageIndex],
				vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent), clearValues);
			commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
			commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
			commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipelineLayout, 0, { *descriptorSet },
				nullptr);

			commandBuffer.bindVertexBuffers(0, { *vertexBufferData.buffer }, { 0 });
			commandBuffer.setViewport(
				0, vk::Viewport(0.0f, 0.0f, static_cast<float>(surfaceData.extent.width),
					static_cast<float>(surfaceData.extent.height), 0.0f, 1.0f));
			commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), surfaceData.extent));

			commandBuffer.draw(12 * 3, 1, 0, 0);
			commandBuffer.endRenderPass();
			commandBuffer.end();

			vk::raii::Fence drawFence(device, vk::FenceCreateInfo());

			vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
			vk::SubmitInfo submitInfo(*imageAcquiredSemaphore, waitDestinationStageMask, *commandBuffer);
			graphicsQueue.submit(submitInfo, *drawFence);

			while (vk::Result::eTimeout == device.waitForFences({ *drawFence }, VK_TRUE, vk::su::FenceTimeout));

			vk::PresentInfoKHR presentInfoKHR(nullptr, *swapChainData.swapChain, imageIndex);
			result = presentQueue.presentKHR(presentInfoKHR);
			switch (result)
			{
			case vk::Result::eSuccess: break;
			case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
				break;
			default: assert(false); // an unexpected result is returned !
			}
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		/* VULKAN_KEY_END */

		device.waitIdle();
	}
	catch (vk::SystemError& err)
	{
		std::cout << "vk::SystemError: " << err.what() << std::endl;
		exit(-1);
	}
	catch (std::exception& err)
	{
		std::cout << "std::exception: " << err.what() << std::endl;
		exit(-1);
	}
	catch (...)
	{
		std::cout << "unknown error\n";
		exit(-1);
	}
	return 0;
}
