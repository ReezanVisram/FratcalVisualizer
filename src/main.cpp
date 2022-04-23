#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Fractal.h"
#include "Mandelbrot.h"
#include "Dragon.h"
#include "Snowflake.h"
#include "SierpinskiTriangle.h"
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void displayFps(int frameCount);
void updateActiveFractal(Fractal **activeFractal, const char* newFractal, const std::map<const char*, Fractal*> map);

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
    SierpinskiTriangle* triangle = new SierpinskiTriangle(10);

    std::map<const char*, Fractal*> fractals = {
        {"Mandelbrot Set", mandelbrot},
        {"Heighway Dragon", dragon},
        {"Koch's Snowflake", snowflake},
        {"Sierpinski's Triangle", triangle}
    };

    std::map<const char*, Fractal*>::iterator fractalsItr;

    int activeIndex = 3;
    Fractal* activeFractal = fractals.at("Mandelbrot Set");
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

    float* triangleVertices = triangle->ConvertVertices();
    unsigned int* triangleIndices = triangle->ConvertIndices();
    VertexArray triangleVertexArray;
    VertexBuffer triangleVertexBuffer(triangleVertices, triangle->GetNumIndices() * 2 * sizeof(float));
    IndexBuffer triangleIndexBuffer(triangleIndices, triangle->GetNumIndices());
    VertexBufferLayout triangleLayout;
    triangleLayout.Push<float>(2);
    triangleVertexArray.AddBuffer(triangleVertexBuffer, triangleLayout);
    Shader triangleShader = Shader(triangle->GetVertexPath(), triangle->GetFragmentPath());

    Renderer renderer;

    float backgroundRed = 0.0f;
    float backgroundGreen = 0.0f;
    float backgroundBlue = 0.0f;

    float fractalRed = 1.0f;
    float fractalGreen = 1.0f;
    float fractalBlue = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        prevActiveIndex = activeIndex;
        prevGenerations = generations;
        processInput(window);
        std::vector<float> screenSize = { float(SCR_WIDTH), float(SCR_HEIGHT) };
        std::vector<float> controls = { CENTER_X, CENTER_Y, ZOOM };

        std::vector<float> color = { fractalRed, fractalGreen, fractalBlue, 1.0f };

        mandelbrotShader.SetFloatUniform("WindowSize", screenSize);
        mandelbrotShader.SetFloatUniform("Controls", controls);
        dragonShader.SetFloatUniform("Color", color);
        snowflakeShader.SetFloatUniform("Color", color);

        renderer.ClearColor(backgroundRed, backgroundGreen, backgroundBlue, 1.0f);
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
        else if (activeFractal->GetDrawType() == 3) {
            triangleVertexArray.Bind();
            triangleShader.Bind();
            triangleIndexBuffer.Bind();
            renderer.DrawArrays(triangleVertexArray, triangleShader, activeFractal->GetEnumType(), 0, activeFractal->GetNumIndices());
        }

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Fractals")) {
                for (fractalsItr = fractals.begin(); fractalsItr != fractals.end(); ++fractalsItr) {
                    if (ImGui::MenuItem(fractalsItr->first)) {
                        updateActiveFractal(&activeFractal, fractalsItr->first, fractals);
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Colors")) {
                if (ImGui::MenuItem("*Note that changing colours does not affect the Mandelbrot Set as its colours are based on mangification level!*")) {}
                if (ImGui::BeginMenu("Background Colour")) {
                    ImGui::SliderFloat("Red", &backgroundRed, 0.0f, 1.0f);
                    ImGui::SliderFloat("Green", &backgroundGreen, 0.0f, 1.0f);
                    ImGui::SliderFloat("Blue", &backgroundBlue, 0.0f, 1.0f);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Fractal Colour")) {
                    ImGui::SliderFloat("Red", &fractalRed, 0.0f, 1.0f);
                    ImGui::SliderFloat("Green", &fractalGreen, 0.0f, 1.0f);
                    ImGui::SliderFloat("Blue", &fractalBlue, 0.0f, 1.0f);
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("About")) {
                if (ImGui::BeginMenu("Instructions")) {
                    if (ImGui::BeginMenu("Mandelbrot Set")) {
                        ImGui::Text("Pan around using your arrow keys!\nZoom in with Left CTRL and zoom out with Left SHIFT\nNote that changing colours does not work for the Mandelbrot Set as its colours are determined based on magnification level");
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Development Information")) {
                    ImGui::Text("FractalVisualizer was made by Reezan Visram as a project to explore OpenGL and chaos mathematics.\nPlease check out my website at reezanvisram.com for more projects!");
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("More Fractals?")) {
                    ImGui::Text("More fractals and more customization options (such as changing the number of Mandelbrot Set iterations or drawing multiple fractals on top of one another)\n are coming! Stay tuned");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
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

void updateActiveFractal(Fractal **activeFractal, const char* newFractal, const std::map<const char*, Fractal*> map) {
    *activeFractal = map.at(newFractal);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}