#include "tool.hpp"

// 初始化外部变量（确保它们在一个 .cpp 文件中定义）
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = width / 2;  // 默认窗口宽度的一半
float lastY = height / 2;  // 默认窗口高度的一半
Camera myCamera(glm::vec3(1.0, 2.5, 1.0), glm::vec3(0.0, 1.0, 0.0), 60.0f, 0.0f);
glm::mat4 projection= glm::perspective(glm::radians(myCamera.GetZoom()), (float)width / (float)height, 0.1f, 100.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    float currentX = static_cast<float>(xPos);
    float currentY = static_cast<float>(yPos);

    if (firstMouse) {
        lastX = currentX;
        lastY = currentY;
        firstMouse = false;
    }

    float xoffset = currentX - lastX;
    float yoffset = lastY - currentY;
    lastX = currentX;
    lastY = currentY;
    myCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    myCamera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        myCamera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        myCamera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        myCamera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        myCamera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    }
}

void initWorkpieceData(ZMap &workpiece){
    for (int x = 0; x < workpiece.length; x++)
    {
        for (int z = 0; z < workpiece.width; z++)
        {
            workpiece.setDepth(x, z, cos(x * workpiece.precision) + sin(z * workpiece.precision));
        }
    }
}