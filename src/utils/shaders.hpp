#pragma once
#include <fstream>
#include <vulkan/vulkan.hpp>

#include "raii.hpp"


static auto readFile(const std::string& filename) -> std::vector<char>
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	// ate: start reading at the end of the file
	// the advantage of this is that we can use the read position to determine the size of the file and allocate a buffer
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	size_t fileSize = (size_t)file.tellg();
	// tellg: get the position of the current character in the input stream
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

namespace vk
{
	namespace raii
	{
		namespace su
		{
			template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
			vk::raii::ShaderModule makeShaderModule(vk::raii::Device const& device, const std::string& filename)
			{
				auto code = readFile(filename);
				std::vector<uint32_t> uint32Vec;
				uint32Vec.resize(code.size() / 4); // make sure charVec.size() is a multiple of 4
				std::memcpy(uint32Vec.data(), code.data(), code.size());
				return vk::raii::ShaderModule{
					device, vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(),
					                                   uint32Vec)
				};
			}
		} // namespace su
	} // namespace raii
} // namespace vk
