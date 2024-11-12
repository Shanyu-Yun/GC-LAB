
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "Zmap.hpp"
const float width = 1200.0;
const float height = 1200.0;
// 声明外部变量
extern float deltaTime;
extern float lastFrame;
extern bool firstMouse;
extern float lastX;
extern float lastY;
extern Camera myCamera;
extern glm::mat4 projection;
// 回调函数声明
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void initWorkpieceData(ZMap &data);