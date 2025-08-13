#include "imguiUI.hpp"

ImguiUI::ImguiUI() {}

ImguiUI::~ImguiUI() {}

void ImguiUI::init(GLFWwindow* window, const std::string& glsl_version) {
    m_window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO();

    m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Navigation

    ImGui::StyleColorsDark();

    std::string version = "#version " + glsl_version;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(version.c_str());
}

void ImguiUI::beginRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_io = ImGui::GetIO();

    // Docking setup
    m_viewport = ImGui::GetMainViewport();
    ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(m_viewport->ID, m_viewport, docking_flags);

    renderPanels();

    ImGui::Render();
}

void ImguiUI::render() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //GLFWwindow* backup_current_context = glfwGetCurrentContext();
    //ImGui::UpdatePlatformWindows();
    //ImGui::RenderPlatformWindowsDefault();
    //glfwMakeContextCurrent(backup_current_context);
}

void ImguiUI::renderPanels() {
    infoBoard();
}

void ImguiUI::infoBoard() {
    ImGui::Begin("Stats");
    ImGui::Text("FPS : %.1f", m_io.Framerate);
    ImGui::End();
}