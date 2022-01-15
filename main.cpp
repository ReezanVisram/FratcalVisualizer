#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Fractal.h"
#include "Mandelbrot.h"
#include "Dragon.h"
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void displayFps(int frameCount);

unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

float CENTER_X = 0.0f;
float CENTER_Y = 0.0f;
float ZOOM = 1.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fractal", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Mandelbrot mandelbrot;

    Dragon dragon(20);

    float* vertices = dragon.ConvertVertices();

    unsigned int* indices = dragon.ConvertIndices();

    VertexArray va;   
    VertexBuffer vb(vertices, dragon.GetNumIndices() * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);



    Shader activeShader = Shader("./dragon.vert", "./dragon.frag");
    
    Renderer renderer;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //std::vector<float> screenSize = { float(SCR_WIDTH), float(SCR_HEIGHT) };
        //std::vector<float> controls = { CENTER_X, CENTER_Y, ZOOM };

        //activeShader.SetFloatUniform("WindowSize", screenSize);
        //activeShader.SetFloatUniform("Controls", controls);

        va.Bind();
        activeShader.Bind();


        renderer.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.ClearBit(GL_COLOR_BUFFER_BIT);
       /* renderer.Draw(va, ib, activeShader, GL_LINE_STRIP);*/
        
        glDrawArrays(GL_LINE_STRIP, 0, dragon.GetNumIndices());
        //double currentTime = glfwGetTime();
        //frameCount++;

        //if (currentTime - previousTime >= 1.0f) {
        //    displayFps(frameCount);

        //    frameCount = 0;
        //    previousTime = currentTime;
        //}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void displayFps(int frameCount) {
    std::cout << frameCount << " FPS" << std::endl;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        CENTER_Y += (0.005f * ZOOM);

        if (CENTER_Y > 1.0f) {
            CENTER_Y = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        CENTER_Y -= (0.005f * ZOOM);

        if (CENTER_Y < -1.0f) {
            CENTER_Y = -1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        CENTER_X -= (0.005f * ZOOM);

        if (CENTER_X < -1.0f) {
            CENTER_X = -1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        CENTER_X += (0.005f * ZOOM);

        if (CENTER_X > 1.0f) {
            CENTER_X = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        ZOOM *= 1.02f;

        if (ZOOM > 1.0f) {
            ZOOM = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        ZOOM *= 0.98f;

        if (ZOOM < 0.00000001f) {
            ZOOM = 0.0000001f;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}