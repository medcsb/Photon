#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Photon/Renderer.h"

#include "Shader.hpp"
#include "Camera.hpp"
#include "imguiUI.hpp"
#include "Scene.hpp"

#include <vector>

const int OPENGL_VERSION_MAJOR = 4;
const int OPENGL_VERSION_MINOR = 5;
const int GLSL_VERSION = OPENGL_VERSION_MAJOR * 100 + OPENGL_VERSION_MINOR * 10;

struct UI_Render_Info {
    GLuint main_texture;
};

class Engine {
private:
    struct Window {
        GLFWwindow* handle = nullptr;
        int width = 800;
        int height = 600;
    } m_window;

    Renderer m_renderer{};
    ImguiUI m_ui{};
    Scene m_scene{};

    Camera m_camera;
    CameraController m_cameraController{m_camera};

    double m_inputTime = 0.0;

public:
    Engine();
    ~Engine();

    void run();

private:
    void initWindow();
    void initOpenGL();

    void renderMainBuffer();

    void processInput(GLFWwindow* window, float deltaTime);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};