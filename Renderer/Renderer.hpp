#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "systems/Simple_RS.hpp"
#include "systems/PBR_RS.hpp"

#include <vector>

struct FrameBuffer {
    GLuint fbo = 0;
    GLuint colorBuffer = 0;
    GLuint depthBuffer = 0;
};

class Renderer {
private:
    FrameBuffer m_mainFrame;

    RenderInfo m_renderInfo;

    Simple_RS m_simpleRenderSystem;
    //PBR_RS m_pbrRenderSystem;

public:
    Renderer();
    ~Renderer();

    void cleanup();

    void setSimpleRenderables(std::vector<SimpleRenderable>* renderables) {
        m_simpleRenderSystem.setRenderables(renderables);
    }

    void initFrameBuffer(uint32_t width, uint32_t height);
    void initShaders(GLuint shaderProg) { m_simpleRenderSystem.init(shaderProg); }

    GLuint getMainFrameColor() const { return m_mainFrame.colorBuffer; }
    
    void renderToFbo(uint32_t width, uint32_t height);
    void renderToScreen(uint32_t width, uint32_t height);
    void setRenderInfo(const RenderInfo& renderInfo) { m_renderInfo = renderInfo; }

    void initTestTriangle();
    
private:
    void render();

    FrameBuffer createFrameBuffer(uint32_t width, uint32_t height);

};