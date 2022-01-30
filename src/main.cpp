#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include "Snowflake.h"
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void displayFps(int frameCount);

void BindVertexArray(VertexArray va);

VertexBuffer createVertexBuffer(Fractal* fractal);
IndexBuffer createIndexBuffer(Fractal* fractal);
Shader createActiveShader(Fractal* fractal);

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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGui::StyleColorsDark();


    Mandelbrot* mandelbrot = new Mandelbrot();
    float generations = 20;
    float prevGenerations = generations;
    Dragon* dragon = new Dragon(static_cast<unsigned int>(generations), 0);

    Snowflake* snowflake = new Snowflake(5);

    Fractal* fractals[] = {
        mandelbrot,
        dragon,
        snowflake
    };

    const char* fractalNames[] = {
        "Mandelbrot Set",
        "Heighway Dragon",
        "Snowflake"
    };

    int activeIndex = 2;
    Fractal* activeFractal = fractals[2];
    int prevActiveIndex = activeIndex;


    float* mandelbrotVertices = mandelbrot->ConvertVertices();
    unsigned int* mandelbrotIndices = mandelbrot->ConvertIndices();
    VertexArray mandelbrotVertexArray;   
    VertexBuffer mandelbrotVertexBuffer(mandelbrotVertices, mandelbrot->GetNumIndices() * 2 * sizeof(float));
    IndexBuffer mandelbrotIndexBuffer(mandelbrotIndices, mandelbrot->GetNumIndices());
    VertexBufferLayout mandelbrotLayout;
    mandelbrotLayout.Push<float>(2);
    mandelbrotVertexArray.AddBuffer(mandelbrotVertexBuffer, mandelbrotLayout);
    Shader mandelbrotShader = Shader(mandelbrot->GetVertexPath(), mandelbrot->GetFragmentPath());
    
    float* dragonVertices = dragon->ConvertVertices();
    unsigned int* dragonIndices = dragon->ConvertIndices();
    VertexArray dragonVertexArray;
    VertexBuffer dragonVertexBuffer(dragonVertices, dragon->GetNumIndices() * 2 * sizeof(float));
    IndexBuffer dragonIndexBuffer(dragonIndices, dragon->GetNumIndices());
    VertexBufferLayout dragonLayout;
    dragonLayout.Push<float>(2);
    dragonVertexArray.AddBuffer(dragonVertexBuffer, dragonLayout);
    Shader dragonShader = Shader(dragon->GetVertexPath(), dragon->GetFragmentPath());

    float* snowflakeVertices = snowflake->ConvertVertices();
    unsigned int* snowflakeIndices = snowflake->ConvertIndices();
    VertexArray snowflakeVertexArray;
    VertexBuffer snowflakeVertexBuffer(snowflakeVertices, snowflake->GetNumIndices() * 2 * sizeof(float));
    IndexBuffer snowflakeIndexBuffer(snowflakeIndices, snowflake->GetNumIndices());
    VertexBufferLayout snowflakeLayout;
    snowflakeLayout.Push<float>(2);
    snowflakeVertexArray.AddBuffer(snowflakeVertexBuffer, snowflakeLayout);
    Shader snowflakeShader = Shader(snowflake->GetVertexPath(), snowflake->GetFragmentPath());


    Renderer renderer;

    std::vector<float> color = { 0.0f, 0.0f, 1.0f, 1.0f };

    while (!glfwWindowShouldClose(window))
    {
        prevActiveIndex = activeIndex;
        prevGenerations = generations;
        processInput(window);
        std::vector<float> screenSize = { float(SCR_WIDTH), float(SCR_HEIGHT) };
        std::vector<float> controls = { CENTER_X, CENTER_Y, ZOOM };

        mandelbrotShader.SetFloatUniform("WindowSize", screenSize);
        mandelbrotShader.SetFloatUniform("Controls", controls);
        dragonShader.SetFloatUniform("Color", color);
        snowflakeShader.SetFloatUniform("Color", color);

        renderer.ClearColor(0.4f, 0.97f, 1.0f, 1.0f);
        renderer.ClearBit(GL_COLOR_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (activeFractal->GetDrawType() == 0) {
            mandelbrotVertexArray.Bind();
            mandelbrotShader.Bind();
            mandelbrotIndexBuffer.Bind();
            renderer.DrawElements(mandelbrotVertexArray, mandelbrotIndexBuffer, mandelbrotShader, activeFractal->GetEnumType());
        }
        else if (activeFractal->GetDrawType() == 1) {
            ImGui::SliderFloat("Generations", &generations, 0.0f, 25.0f);
            ImGui::Button("Draw");
            dragonVertexArray.Bind();
            dragonShader.Bind();
            dragonIndexBuffer.Bind();
            renderer.DrawArrays(dragonVertexArray, dragonShader, activeFractal->GetEnumType(), 0, activeFractal->GetNumIndices());
        }
        else if (activeFractal->GetDrawType() == 2) {
            snowflakeVertexArray.Bind();
            snowflakeShader.Bind();
            snowflakeIndexBuffer.Bind();
            renderer.DrawArrays(snowflakeVertexArray, snowflakeShader, activeFractal->GetEnumType(), 0, activeFractal->GetNumIndices());
        }

        ImGui::Begin("Fractal");
        ImGui::ListBox("Active Fractal", &activeIndex, fractalNames, 3);
        ImGui::Button("Draw");

        ImGui::End();

        if (activeIndex != prevActiveIndex) {
            activeFractal = fractals[activeIndex];
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    delete mandelbrot;
    delete dragon;
    mandelbrot = nullptr;
    dragon = nullptr;
    glfwTerminate();
    return 0;
}

VertexBuffer createVertexBuffer(Fractal* fractal) {
    float* vertices = fractal->ConvertVertices();
    VertexBuffer vb(vertices, fractal->GetNumIndices() * 2 * sizeof(float));
    return vb;
}

IndexBuffer createIndexBuffer(Fractal* fractal) {
    unsigned int* indices = fractal->ConvertIndices();
    IndexBuffer ib(indices, fractal->GetNumIndices());
    return ib;
}

Shader createActiveShader(Fractal* fractal) {
    Shader shader(fractal->GetVertexPath(), fractal->GetFragmentPath());
    return shader;
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