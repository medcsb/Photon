#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Buffer.hpp"

#include "RenderStructs.hpp"

// RS stands for Render System

struct SimpleMaterial {
    glm::vec3 baseColor;
    float ambient;
    float diffuse;
    float specular;
    float specStrength;
    float specPower;
};

struct SimpleRenderable {
    MeshBuffer meshBuffer;
    SimpleMaterial material;
    glm::mat4 transform;
};

class Simple_RS {
private:
    GLuint m_shaderProgram;
    std::vector<SimpleRenderable>* m_renderableQueue;
public:
    Simple_RS();
    ~Simple_RS();

    void cleanup();

    void init(GLuint shaderProgram) { m_shaderProgram = shaderProgram; }

    void setRenderables(std::vector<SimpleRenderable>* renderables) { m_renderableQueue = renderables; }

    void render(RenderInfo& renderInfo);
};