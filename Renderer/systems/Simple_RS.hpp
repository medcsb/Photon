#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Buffer.hpp"

#include "RenderStructs.hpp"

// RS stands for Render System

class Simple_RS {
private:
    GLuint m_shaderProgram;
    std::vector<SimpleRenderable> m_renderableQueue;
public:
    Simple_RS();
    ~Simple_RS();

    void init(GLuint shaderProgram) { m_shaderProgram = shaderProgram; }

    void add_toQueue(const SimpleRenderable& renderable);
    void flush();

    void render(RenderInfo& renderInfo);
};