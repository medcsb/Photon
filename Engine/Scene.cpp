#include "Scene.hpp"


Scene::Scene() {}
Scene::~Scene() {}

void Scene::init() {
    std::vector<Vertex> vertices = createTestTriangle();
    VAOConfig config = createConfig();
    meshBuffers.emplace_back(Buffer::createMeshBuffer(config, vertices.data(), nullptr));
    SimpleMaterial material;
    material.baseColor = {1.0f, 0.5f, 0.2f};
    material.ambient = 0.1f;
    material.diffuse = 0.8f;
    material.specular = 0.5f;
    material.specStrength = 0.5f;
    material.specPower = 32.0f;
    materials.push_back(material);
    SimpleRenderable renderable;
    renderable.meshBuffer = meshBuffers.back();
    renderable.material = materials.back();
    renderable.transform = glm::mat4(1.0f);
    renderables.push_back(renderable);

    mainLight.pos = {0.0f, 1.0f, -3.0f};
    mainLight.color = {1.0f, 1.0f, 1.0f};

    renderInfo.lights.push_back(mainLight);

}

VAOConfig Scene::createConfig() {
    VAOConfig config;
    
    config.attributes.push_back({0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, pos)});
    config.attributes.push_back({1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal)});

    config.size_vertex = sizeof(Vertex);
    config.num_vertices = 3; 
    config.index_count = UINT32_MAX;
    config.draw_mode = GL_TRIANGLES;
    config.usage = GL_DYNAMIC_DRAW;

    return config;
}