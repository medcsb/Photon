#include "Scene.hpp"


Scene::Scene() {}
Scene::~Scene() {}

void Scene::AddCubeObj() {

    if (m_renderables.capacity() <= m_objCount) {
        m_renderables.resize(m_objCount + 1);
    }

    Obj obj;
    obj.idx = m_objCount++;
    m_objNames.push_back("Cube " + std::to_string(obj.idx + 1));
    obj.type = RenderType::Simple;
    m_objects.push_back(obj);
    
    Model cubeModel;
    cubeModel.CubeModel();
    m_models.push_back(cubeModel);
    
    VAOConfig config = createConfig(obj.idx);

    m_renderables[obj.idx].meshBuffer = Buffer::createMeshBuffer(
        config,
        m_models[obj.idx].getVertices().data(),
        m_models[obj.idx].getIndices().data()
    );

    m_renderables[obj.idx].material = {
        .baseColor = {1.0f, 0.5f, 0.2f},
        .ambient = 0.1f,
        .diffuse = 0.8f,
        .specular = 0.5f,
        .specStrength = 0.5f,
        .specPower = 32.0f
    };

    m_renderables[obj.idx].transform = glm::mat4(1.0f);

    Light mainLight;
    
    mainLight.pos = {0.0f, 1.0f, 3.0f};
    mainLight.color = {1.0f, 1.0f, 1.0f};
    
    m_renderInfo.lights.push_back(mainLight);
}


VAOConfig Scene::createConfig(size_t idx) {
    VAOConfig config;
    
    config.attributes.push_back({0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, pos)});
    config.attributes.push_back({1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal)});
    config.attributes.push_back({2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, texCoords)});

    config.size_vertex = sizeof(Vertex);
    config.num_vertices = m_models[idx].getVertices().size();
    config.index_count = m_models[idx].getIndices().size();
    config.draw_mode = GL_TRIANGLES;
    config.usage = GL_DYNAMIC_DRAW;

    return config;
}