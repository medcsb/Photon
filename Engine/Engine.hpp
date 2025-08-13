#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Photon/Renderer.h"

#include "Shader.hpp"
#include "Camera.hpp"
#include "imguiUI.hpp"
#include <vector>

const int OPENGL_VERSION_MAJOR = 4;
const int OPENGL_VERSION_MINOR = 5;
const int GLSL_VERSION = OPENGL_VERSION_MAJOR * 100 + OPENGL_VERSION_MINOR * 10;

class Engine {
private:
    struct Window {
        GLFWwindow* handle = nullptr;
        int width = 800;
        int height = 600;
    } m_window;

    Renderer m_renderer;
    //std::vector<Shader> m_shaders;

    ImguiUI ui{};

    Camera m_camera;
    CameraController m_cameraController{m_camera};

public:
    Engine();
    ~Engine();

    void run();

private:
    void initWindow();
    void initOpenGL();

    void processInput(GLFWwindow* window, float deltaTime);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};