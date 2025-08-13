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
    ImGuiIO m_io;
public:
    ImguiUI();
    ~ImguiUI();

    void init(GLFWwindow* window , const std::string& glsl_version);

    void beginRender();
    void render();

private:
    void renderPanels();

    void infoBoard();
};