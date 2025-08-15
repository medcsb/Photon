#include "Engine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils/log.h"

#include <stdexcept>

Engine::Engine() {
    initWindow();
    initOpenGL();
    m_renderer.initFrameBuffer(m_window.width, m_window.height);
    m_ui.init(m_window.handle , std::to_string(GLSL_VERSION));
    m_scene.init();
    Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");
    simpleShader.init();
    m_renderer.initShaders(simpleShader.getProgramId());
    m_renderer.add_toQueue(m_scene.renderables[0]);
}

Engine::~Engine() {

}

void Engine::run() {
    while(!glfwWindowShouldClose(m_window.handle)) {
        processInput(m_window.handle, ImGui::GetIO().DeltaTime);

        m_scene.renderInfo.viewMatrix = m_camera.getViewMatrix();
        m_scene.renderInfo.projectionMatrix = m_camera.getProjectionMatrix();

        m_camera.updateProjectionMatrix(m_window.width, m_window.height);
        m_camera.updateViewMatrix();

        m_renderer.setRenderInfo(m_scene.renderInfo);
        
        m_renderer.render();
        m_ui.beginRender();
        renderMainBuffer();
        m_ui.render();
        m_ui.endRender();
        
        glfwSwapBuffers(m_window.handle);
        glfwPollEvents();
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

void Engine::renderMainBuffer() {
    ImGui::Begin("Scene");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImGui::Image((ImTextureID)(intptr_t)m_renderer.getMainFrameColor(), avail, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void Engine::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetTime() - m_inputTime > 0.016) {
        m_inputTime = glfwGetTime();
        m_cameraController.processMouseInput(window, deltaTime);
    }
    m_cameraController.processKeyboardInput(window, deltaTime);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
