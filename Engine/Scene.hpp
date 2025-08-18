#pragma once

#include "Photon/Renderer.h"
#include "Shader.hpp"
#include "Model.hpp"

struct Obj {
    RenderType type;
    uint32_t idx;
};

class Scene {
private:

    uint32_t m_objCount = 0;

    std::vector<Shader> m_shaderPrograms;

    std::vector<SimpleRenderable> m_renderables;
    std::vector<Obj> m_objects;
    std::vector<Model> m_models;
    std::vector<std::string> m_objNames;
    std::vector<Light> m_lights;
    RenderInfo m_renderInfo;
    
public:
    Scene();
    ~Scene();

    void cleanup();

    void setViewMatrix(const glm::mat4& view) { m_renderInfo.viewMatrix = view; }
    void setProjectionMatrix(const glm::mat4& projection) { m_renderInfo.projectionMatrix = projection; }

    RenderInfo& getRenderInfo() { return m_renderInfo; }
    std::vector<std::string>* getObjNames() { return &m_objNames; }
    std::vector<SimpleRenderable>* getRenderables() { return &m_renderables; }

    void AddCubeObj();

    void initExample();

private:
    VAOConfig createConfig(size_t idx);
};

