#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

// Define the structure to store vertex information
struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

// Define the structure to store instance information
struct InstanceData
{
    glm::vec3 offset;
    glm::vec3 coloroffset;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};