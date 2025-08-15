#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "RenderStructs.hpp"

#include "Buffer.hpp"
#include "systems/Simple_RS.hpp"
#include "systems/PBR_RS.hpp"

#include <vector>

class Renderer {
private:
    FrameBuffer m_mainFrame;

    RenderInfo m_renderInfo;

    Simple_RS m_simpleRenderSystem;
    //PBR_RS m_pbrRenderSystem;

    GLuint m_testVAO;
    GLuint m_testVBO;
    GLuint m_testShaderProgram;

public:
    Renderer();
    ~Renderer();

    void add_toQueue(const SimpleRenderable& renderable) {
        m_simpleRenderSystem.add_toQueue(renderable);
    }

    void initFrameBuffer(uint32_t width, uint32_t height);
    void initShaders(GLuint shaderProg) {
        m_simpleRenderSystem.init(shaderProg);
    }

    GLuint getMainFrameColor() const { return m_mainFrame.colorBuffer; }
    
    void render();
    void setRenderInfo(const RenderInfo& renderInfo) { m_renderInfo = renderInfo; }

    void initTestTriangle();
    
private:
    FrameBuffer createFrameBuffer(uint32_t width, uint32_t height);

};