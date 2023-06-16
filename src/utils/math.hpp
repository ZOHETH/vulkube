#pragma once

#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS

#if defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4127 )  // conditional expression is constant (glm)
#endif

#include <glm/gtc/matrix_transform.hpp>

namespace vk
{
    namespace su
    {
        glm::mat4x4 createModelViewProjectionClipMatrix(vk::Extent2D const& extent);
    }
}  // namespace vk
