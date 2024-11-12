#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "Zmap.hpp"
#include "tool.hpp"
#include "config.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(width, height, "Z-Map", nullptr, nullptr);

    if (window == nullptr)
    {
        throw std::runtime_error("创建窗口失败");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("加载glad失败");
    }
    //面剔除以及设置线宽
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glLineWidth(4.0f);

    // 初始化ZMap，并用一个二元函数初始化其数值
    // 长度、宽度与精度
    ZMap workpiece(100,100, 0.1);
    initWorkpieceData(workpiece);
    workpiece.depthToCoords();
    workpiece.generateIndices();
    workpiece.generateLineIndices();

    //读取着色器文件，并生成着色器程序
    std::string vertShaderPath = std::string(ASSETS_PATH) + "/workpieceshader.vert";
    std::string fragShaderPath = std::string(ASSETS_PATH) + "/workpieceshader.frag";
    std::string linefragShaderPath = std::string(ASSETS_PATH) + "/workpiecelineshader.frag";
    std::string linevertShaderPath = std::string(ASSETS_PATH) + "/workpiecelineshader.vert";
    Shader workpieceShader(vertShaderPath.c_str(), fragShaderPath.c_str());
    Shader workpiecelineShader(linevertShaderPath.c_str(), linefragShaderPath.c_str());
    glEnable(GL_DEPTH_TEST);

    GLuint workpieceVBO, workpieceVAO, workpieceEBO,lineVAO,lineVBO,lineEBO;
    glGenBuffers(1, &workpieceVBO);
    glGenBuffers(1, &workpieceEBO);
    glGenVertexArrays(1, &workpieceVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);
    glGenVertexArrays(1, &lineVAO);

    glBindVertexArray(workpieceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, workpieceVBO);
    glBufferData(GL_ARRAY_BUFFER, workpiece.zmapCoords.size() * sizeof(float), workpiece.zmapCoords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, workpieceEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, workpiece.zmapIndices.size() * sizeof(int), workpiece.zmapIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, workpiece.zmapCoords.size() * sizeof(float), workpiece.zmapCoords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, workpiece.lineIndices.size() * sizeof(int), workpiece.lineIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.2f,0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        workpieceShader.use();
        workpieceShader.setMat4("Projection", projection);
        workpieceShader.setMat4("View", myCamera.GetViewMatrix());
        workpieceShader.setMat4("Model", ModelMatrix);
        //绘制workpiece
        glBindVertexArray(workpieceVAO);
        glDrawElements(GL_TRIANGLES, workpiece.zmapIndices.size(), GL_UNSIGNED_INT, 0);
        
        
        //绘制workplace网格
        workpiecelineShader.use();
        workpiecelineShader.setMat4("Projection", projection);
        workpiecelineShader.setMat4("View", myCamera.GetViewMatrix());
        workpiecelineShader.setMat4("Model", ModelMatrix);
        workpiecelineShader.setVec3("Colors",glm::vec3(0.4,0.4,0.4));
        glBindVertexArray(lineVAO);
        glDrawElements(GL_LINES, workpiece.lineIndices.size(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
