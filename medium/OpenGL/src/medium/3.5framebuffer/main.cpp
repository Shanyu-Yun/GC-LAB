#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// Vertex shader source code
const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 position;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(position,0.0, 1.0);
    TexCoord = position * 0.5 + 0.5; // Normalize to [0, 1]
})";

// Fragment shader for rendering the blue triangle
const char *triangleFragmentShaderSource = R"(
#version 330 core
out vec4 color;

void main() {
    color = vec4(1.0, 0.0, 1.0, 1.0); // Blue color
})";

// Fragment shader for rendering the white rectangle
const char *rectangleFragmentShaderSource = R"(
#version 330 core
out vec4 color;

void main() {
    color = vec4(1.0, 1.0, 1.0, 1.0); // White color
})";

// Fragment shader for blur effect
const char *blurFragmentShaderSource = R"(
#version 330 core
out vec4 color;
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float blurAmount;

void main() {
    vec2 texOffset = 1.0 / textureSize(screenTexture, 0); // Get texture coordinates offset
    vec4 result = vec4(0.0);
    
    // Sample the texture in a larger grid around the current fragment
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            result += texture(screenTexture, TexCoord + vec2(x, y) * texOffset * blurAmount);
        }
    }
    color = result / 25.0; // Average the samples
})";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint createShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}

GLuint createProgram(const char *vertexSource, const char *fragmentSource)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Blurred Triangle", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    GLuint rectangleShaderProgram = createProgram(vertexShaderSource, rectangleFragmentShaderSource);
    GLuint triangleShaderProgram = createProgram(vertexShaderSource, triangleFragmentShaderSource);
    GLuint blurShaderProgram = createProgram(vertexShaderSource, blurFragmentShaderSource);

    float rectangleVertices[] = {
        -0.8f, 0.6f, -0.5f,  // Top-left
        -0.8f, -0.6f, -0.5f, // Bottom-left
        0.6f, -0.6f, -0.5f,  // Bottom-right
        0.6f, 0.6f, -0.5f,   // Top-right
    };
    unsigned int rectangleIndices[] = {0, 1, 2, 0, 2, 3};

    float triangleVertices[] = {
        -0.4f, 0.4f, 0.5f,  // Top
        -0.4f, -0.4f, 0.5f, // Bottom-left
        0.4f, -0.4f, 0.5f,  // Bottom-right
    };
    unsigned int triangleIndices[] = {0, 1, 2};

    GLuint rectangleVAO, rectangleVBO, rectangleEBO;
    glGenVertexArrays(1, &rectangleVAO);
    glGenBuffers(1, &rectangleVBO);
    glGenBuffers(1, &rectangleEBO);

    glBindVertexArray(rectangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint triangleVAO, triangleVBO, triangleEBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);
    glGenBuffers(1, &triangleEBO);

    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create framebuffers and textures
    GLuint framebuffers[2], textures[2];
    glGenFramebuffers(2, framebuffers);
    glGenTextures(2, textures);
    for (int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);

        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer " << i << " is not complete!" << std::endl;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int pingpong = 0;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Bind the framebuffer object to render to it
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[pingpong]);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle to framebuffer
        glUseProgram(triangleShaderProgram);
        glBindVertexArray(triangleVAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Unbind the current framebuffer to switch back to the default window
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Switch to the next framebuffer for ping-pong rendering
        pingpong = 1 - pingpong;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[pingpong]);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(blurShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[1 - pingpong]);
        glUniform1f(glGetUniformLocation(blurShaderProgram, "blurAmount"), 10.0f);
        glBindVertexArray(triangleVAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Switch back to the default framebuffer to prepare for drawing the final scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(rectangleShaderProgram);
        glBindVertexArray(rectangleVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(blurShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[pingpong]);

        // Bind the triangle's VAO
        glBindVertexArray(triangleVAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &rectangleVAO);
    glDeleteBuffers(1, &rectangleVBO);
    glDeleteBuffers(1, &rectangleEBO);
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteBuffers(1, &triangleEBO);
    glDeleteFramebuffers(2, framebuffers);
    glDeleteTextures(2, textures);

    glfwTerminate();
    return 0;
}
