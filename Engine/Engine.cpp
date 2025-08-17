#include "Engine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils/log.h"

#include <stdexcept>
#include <string>

#define ENGINE_MODE

Engine::Engine() {
    initWindow();
    initOpenGL();
    m_renderer.initFrameBuffer(m_mainFboSize.width, m_mainFboSize.height);
    m_ui.init(m_window.handle , std::to_string(GLSL_VERSION));
    m_scene.AddCubeObj();
    Shader simpleShader{std::string(SHADER_DIR) + "simple.vert", std::string(SHADER_DIR) + "simple.frag"};
    simpleShader.init();
    m_renderer.initShaders(simpleShader.getProgramId());
    m_renderer.setSimpleRenderables(m_scene.getRenderables());
    fillUIStruct();
    glEnable(GL_DEPTH_TEST);
}

Engine::~Engine() {

}

void Engine::run() {
    while(!glfwWindowShouldClose(m_window.handle)) {
        processInput(m_window.handle, ImGui::GetIO().DeltaTime);
        if (m_mainFboSize.resized) {
            m_renderer.initFrameBuffer(m_mainFboSize.width, m_mainFboSize.height);
            m_mainFboSize.resized = false;
        }

        
        #ifdef ENGINE_MODE
        m_camera.updateProjectionMatrix(m_mainFboSize.width, m_mainFboSize.height);
        #else
        m_camera.updateProjectionMatrix(m_window.width, m_window.height);
        #endif
        m_camera.updateViewMatrix();
        
        
        m_scene.setViewMatrix(m_camera.getViewMatrix());
        m_scene.setProjectionMatrix(m_camera.getProjectionMatrix());
        m_renderer.setRenderInfo(m_scene.getRenderInfo());

        #ifdef ENGINE_MODE
        m_renderer.renderToFbo(m_mainFboSize.width, m_mainFboSize.height);
        m_ui.beginRender();
        m_ui.render(m_uiStruct);
        m_ui.endRender(); 
        #else
        m_renderer.renderToScreen(m_window.width, m_window.height);
        #endif
        
        glfwSwapBuffers(m_window.handle);
        glfwPollEvents();
    }
}

void Engine::initWindow() {
    if (!glfwInit()) throw std::runtime_error("Failed to init glfw");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS compatibility

    m_window.handle = glfwCreateWindow(m_window.width, m_window.height, "PHOTON", nullptr, nullptr);
    if (!m_window.handle) throw std::runtime_error("Failed to create a glfw window");
    glfwSetWindowUserPointer(m_window.handle, this);
    glfwSetFramebufferSizeCallback(m_window.handle, framebuffer_size_callback);

    glfwMakeContextCurrent(m_window.handle);
}

void Engine::initOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to load opengl functions");
    }
    //glViewport(0, 0, m_window.width, m_window.height);
}

void Engine::fillUIStruct() {
    m_uiStruct.mainFboSize = &m_mainFboSize;
    m_uiStruct.main_fbo_tex = (ImTextureID*)(intptr_t)m_renderer.getMainFrameColor();
    m_uiStruct.renderables = m_scene.getRenderables();
    m_uiStruct.objNames = m_scene.getObjNames();
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
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    engine->resetWindowSize(width, height);
}
