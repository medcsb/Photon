#include "imguiUI.hpp"

ImguiUI::ImguiUI() {}

ImguiUI::~ImguiUI() {}

void ImguiUI::init(GLFWwindow* window, const std::string& glsl_version) {
    m_window = window;
    // glsl version is expected to be in the format "450"
    m_versionMajor = std::stoi(glsl_version.substr(0, 1));
    m_versionMinor = std::stoi(glsl_version.substr(1, 1));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_viewport = ImGui::GetMainViewport();

    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Navigation

    m_dockspaceFlags = ImGuiDockNodeFlags_None;

    m_windowFlags = ImGuiWindowFlags_NoTitleBar
                  | ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoResize
                  | ImGuiWindowFlags_NoMove
                  | ImGuiWindowFlags_NoBringToFrontOnFocus
                  | ImGuiWindowFlags_NoNavFocus;

    ImGui::StyleColorsDark();

    std::string version = "#version " + glsl_version;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(version.c_str());
}

void ImguiUI::beginRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Docking setup
    beginDockSpace();
}

void ImguiUI::render() {
    renderPanels();
    ImGui::Render();
}

void ImguiUI::endRender() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }

}

void ImguiUI::renderPanels() {
    infoBoard();
}

void ImguiUI::infoBoard() {

    current_time = glfwGetTime();
    if (current_time - last_updated_time >= 1.0) {
        fps = ImGui::GetIO().Framerate;
        last_updated_time = current_time;
    }

    ImGui::Begin("Stats");
    // print the used graphics API
    ImGui::Text("Graphics API: OpenGL %d.%d", m_versionMajor, m_versionMinor);
    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
    ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
    ImGui::Text("Version: %s", glGetString(GL_VERSION));
    
    ImGui::Text("FPS : %.1f", fps);
    ImGui::End();
}

void ImguiUI::beginDockSpace() {

    ImGui::SetNextWindowPos(m_viewport->Pos);
    ImGui::SetNextWindowSize(m_viewport->Size);
    ImGui::SetNextWindowViewport(m_viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", nullptr, m_windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_dockspaceFlags);

    ImGui::End();
}