#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

#include <string>

class ImguiUI {
private:
    GLFWwindow* m_window;
    ImGuiViewport* m_viewport;
    ImGuiIO* m_io;

    ImGuiDockNodeFlags m_dockspaceFlags;
    ImGuiWindowFlags m_windowFlags;

    uint32_t m_versionMajor;
    uint32_t m_versionMinor;

    double last_updated_time = 0;
    double current_time = 0;
    float fps = 0.0f;

public:
    ImguiUI();
    ~ImguiUI();

    void init(GLFWwindow* window , const std::string& glsl_version);

    void beginRender();
    void render();
    void endRender();

private:
    void beginDockSpace();

    void renderPanels();

    void infoBoard();
};