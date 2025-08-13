#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

struct Mesh {
    GLuint vao;
    GLsizei indexCount;
};

struct Material {
    GLuint albedoMap;
    GLuint normalMap;
    GLuint metallicMap;
    GLuint roughnessMap;

    glm::vec3 baseColor;
    float metallic;
    float roughness;
};

struct RenderInstance {
    GLuint shaderProg;
    Mesh mesh;
    Material material;
};

class Renderer {
private:
    std::vector<RenderInstance> renderQueue;
    std::vector<GLuint> shaderIds;

public:
    Renderer();
    ~Renderer();

    void add_toQueue(RenderInstance instance);
    void flush();

    void render();
};