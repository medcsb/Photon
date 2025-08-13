#include "Engine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils/log.h"

#include <stdexcept>

Engine::Engine() {
    initWindow();
    initOpenGL();
    ui.init(m_window.handle , std::to_string(GLSL_VERSION));
}

Engine::~Engine() {

}

void Engine::run() {
    while(!glfwWindowShouldClose(m_window.handle)) {
        glfwPollEvents();
        processInput(m_window.handle, ImGui::GetIO().DeltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ui.beginRender();
        m_renderer.render();
        ui.render();

        glfwSwapBuffers(m_window.handle);
    }
}

void Engine::initWindow() {
    if (!glfwInit()) throw std::runtime_error("Failed to init glfw");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window.handle = glfwCreateWindow(m_window.width, m_window.height, "PHOTON", nullptr, nullptr);
    if (!m_window.handle) throw std::runtime_error("Failed to create a glfw window");

    glfwMakeContextCurrent(m_window.handle);
}

void Engine::initOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to load opengl functions");
    }
    glViewport(0, 0, m_window.width, m_window.height);
    glfwSetFramebufferSizeCallback(m_window.handle, Engine::framebuffer_size_callback);
}

void Engine::processInput(GLFWwindow* window, float deltaTime) {
    m_cameraController.processMouseInput(window, deltaTime);
    m_cameraController.processKeyboardInput(window, deltaTime);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
