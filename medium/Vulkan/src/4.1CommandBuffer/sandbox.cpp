#define GLFW_INCLUDE_VULKAN
#include "toy2d.hpp"
#include <glfw/glfw3.h>
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main(int argc, char** argv) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    if (!window)
    {
        throw std::runtime_error("failled to create window");
    }

    uint32_t extensionsCount = 0;
    auto extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

    std::vector<const char*> result;
    result.reserve(extensionsCount); 
    for (uint32_t i = 0; i < extensionsCount; ++i)
    {
        result.emplace_back(extensions[i]);
    }
    toy2d::Init(result,
        [&](VkInstance instance){
            VkSurfaceKHR surface;
            glfwCreateWindowSurface(instance, window, nullptr, &surface);
            return surface;
        }, WIDTH, HEIGHT);

    auto renderer = toy2d::GetRenderer();
    while (!glfwWindowShouldClose(window))
    {
        renderer->Render();
        glfwPollEvents();

    }

    toy2d::Quit();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
