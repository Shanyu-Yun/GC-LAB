#pragma once
#include <vulkan/vulkan.h>
#include <vector>

VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);