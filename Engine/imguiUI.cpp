#include "imguiUI.hpp"

#include <algorithm>

ImguiUI::ImguiUI() {}

ImguiUI::~ImguiUI() {}

void ImguiUI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    m_viewport = nullptr;
    m_io = nullptr;
    m_window = nullptr;
}

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

void ImguiUI::render(UI_Struct& ui_struct) {
    renderScenePanel((ImTextureID)(intptr_t)ui_struct.main_fbo_tex, ui_struct.mainFboSize);
    renderInfoPanel(ui_struct);
    renderEditPanel(ui_struct);
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

void ImguiUI::renderScenePanel(ImTextureID sceneTexture, FboSize* mainFboSize) {
    ImGui::Begin("Scene");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    if (m_prevAvail.x != avail.x || m_prevAvail.y != avail.y) {
        m_prevAvail = avail;
        mainFboSize->resized = true;
        mainFboSize->width = static_cast<uint32_t>(std::max(avail.x, 1.0f));
        mainFboSize->height = static_cast<uint32_t>(std::max(avail.y, 1.0f));
    }
    ImGui::Image(sceneTexture, avail, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void ImguiUI::renderInfoPanel(UI_Struct& ui_struct) {

    ImGui::Begin("Info");
    info();
    settings();
    sceneSettings(ui_struct.objNames, ui_struct.lights);
    shaders(ui_struct.shaders);
    ImGui::End();
}

void ImguiUI::renderEditPanel(UI_Struct& ui_struct) {
    ImGui::Begin("Edit");
    if (m_selectedObjIdx == UINT32_MAX) {
        ImGui::Text("No object selected.");
        ImGui::End();
        return;
    }
    if (ui_struct.objects->at(m_selectedObjIdx).type == RenderType::PBR) {
        if (ImGui::SliderFloat3("Pos", glm::value_ptr(ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.pos), -5.0f, 5.0f)) {
            ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.calcMatrix();
        }
        if (ImGui::SliderFloat3("Rot", glm::value_ptr(ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.rot), -180.0f, 180.0f)) {
            ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.calcMatrix();
        }
        if (ImGui::SliderFloat3("Scale", glm::value_ptr(ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.scale), 0.1f, 5.0f)) {
            ui_struct.pbrRenderables->at(m_selectedObjIdx-1).transform.calcMatrix();
        }
    } else if (ui_struct.objects->at(m_selectedObjIdx).type == RenderType::Simple) {
        if(ImGui::SliderFloat3("Pos", glm::value_ptr(ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.pos), -5.0f, 5.0f)) {
            ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.calcMatrix();
        }
        if(ImGui::SliderFloat3("Rot", glm::value_ptr(ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.rot), -180.0f, 180.0f)) {
            ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.calcMatrix();
        }
        if(ImGui::SliderFloat3("Scale", glm::value_ptr(ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.scale), 0.1f, 5.0f)) {
            ui_struct.simpleRenderables->at(m_selectedObjIdx).transform.calcMatrix();
        }
    }
    if (ImGui::CollapsingHeader("Material")) {
        if (ui_struct.objects->at(m_selectedObjIdx).type == RenderType::PBR) {
            ImGui::ColorPicker4("Base Color", &ui_struct.pbrRenderables->at(m_selectedObjIdx-1).material.ubo.baseColor[0]);
            ImGui::SliderFloat("Metallic", &ui_struct.pbrRenderables->at(m_selectedObjIdx-1).material.ubo.metallic, 0.0f, 1.0f);
            ImGui::SliderFloat("Roughness", &ui_struct.pbrRenderables->at(m_selectedObjIdx-1).material.ubo.roughness, 0.0f, 1.0f);
            ImGui::SliderFloat("Ambient Occlusion", &ui_struct.pbrRenderables->at(m_selectedObjIdx-1).material.ubo.ao, 0.0f, 1.0f);
        } else if (ui_struct.objects->at(m_selectedObjIdx).type == RenderType::Simple) {
            ImGui::ColorPicker4("Color", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.baseColor[0]);
            ImGui::SliderFloat("Ambient", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.specular, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.specStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Power", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.specPower, 1.0f, 128.0f);
            ImGui::SliderFloat("Texture Blend", &ui_struct.simpleRenderables->at(m_selectedObjIdx).material.ubo.texBlend, 0.0f, 1.0f);
        }
    }
    ImGui::End();
}

void ImguiUI::info() {
    current_time = glfwGetTime();
    if (current_time - last_updated_time >= 1.0) {
        fps = ImGui::GetIO().Framerate;
        last_updated_time = current_time;
    }

    if (ImGui::CollapsingHeader("Info Panel")) {
        ImGui::TextWrapped("Graphics API: OpenGL %d.%d \n\n", m_versionMajor, m_versionMinor);
        ImGui::TextWrapped("Renderer: %s\n\n", glGetString(GL_RENDERER));
        ImGui::TextWrapped("Vendor: %s\n\n", glGetString(GL_VENDOR));
        ImGui::TextWrapped("Version: %s\n\n", glGetString(GL_VERSION));
        ImGui::TextWrapped("FPS : %.1f\n\n", fps);
    }
}

void ImguiUI::settings() {
    if (ImGui::CollapsingHeader("Settings")) {
        ImGui::Text("Settings will be implemented in the future.");
        // Add settings options here
    }
}

void ImguiUI::sceneSettings(std::vector<std::string>* objNames, std::vector<Light>* lights) {
    if (ImGui::CollapsingHeader("Scene Settings")) {
        if (ImGui::CollapsingHeader("Objects")) {
            for (size_t i = 0; i < objNames->size(); ++i) {
                if (ImGui::Button(objNames->at(i).c_str())) m_selectedObjIdx = i;
            }
        }

        if (ImGui::CollapsingHeader("Lights")) {
            for (size_t i = 0; i < lights->size(); ++i) {
                std::string lightName = "Light " + std::to_string(i);
                if (ImGui::CollapsingHeader(lightName.c_str())) {
                    ImGui::ColorEdit3("Color", &lights->at(i).color[0]);
                    ImGui::SliderFloat3("Position", &lights->at(i).pos[0], -5.0f, 5.0f);
                }
            }
        }
    }
}

void ImguiUI::shaders(std::vector<Shader>* shaders) {
    if (ImGui::CollapsingHeader("Shaders")) {
        for (size_t i = 0; i < shaders->size(); ++i) {
            std::string str = "Reload " + std::to_string(shaders->at(i).getProgramId());
            if (ImGui::Button(str.c_str())) {
                m_onShaderReload(i);
            }
        }
    }
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
