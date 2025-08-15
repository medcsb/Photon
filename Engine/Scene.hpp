#pragma once

#include "Photon/Renderer.h"
#include "Shader.hpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

class Scene {
public:
    std::vector<MeshBuffer> meshBuffers;
    std::vector<SimpleMaterial> materials;
    std::vector<SimpleRenderable> renderables;
    Light mainLight;
    glm::mat4 triTransform;
    RenderInfo renderInfo;
    
public:
    Scene();
    ~Scene();

    void init();
    VAOConfig createConfig();

    std::vector<Vertex> createTestTriangle() {
        return {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
        };
    }
};

