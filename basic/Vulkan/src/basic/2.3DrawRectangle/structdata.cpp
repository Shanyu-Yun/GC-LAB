#include "structdata.h"

// Set vertex input binding description
VkVertexInputBindingDescription Vertex::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

// Set vertex attribute description
std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

// Set instance input binding description
VkVertexInputBindingDescription InstanceData::getBindingDescription()
{
    VkVertexInputBindingDescription instanceBinding{};
    instanceBinding.binding = 1;
    instanceBinding.stride = sizeof(InstanceData);
    instanceBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE; 
    return instanceBinding;
}

// Set instance attribute description
std::array<VkVertexInputAttributeDescription, 2> InstanceData::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 1;
    attributeDescriptions[0].location = 2;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(InstanceData, offset);

    attributeDescriptions[1].binding = 1;
    attributeDescriptions[1].location = 3;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(InstanceData, coloroffset);

    return attributeDescriptions;
}