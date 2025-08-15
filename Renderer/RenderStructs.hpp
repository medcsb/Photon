#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

struct Light;
struct RenderInfo;
struct SimpleMaterial;
struct SimpleRenderable;
struct FrameBuffer;
struct MeshBuffer;
struct VAOConfig;
struct AttribInfo;

struct MeshBuffer {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    size_t index_count = UINT32_MAX;
    GLenum draw_mode = GL_TRIANGLES;

    // might need to disable copying

    void bind() const {glBindVertexArray(vao);}
    void bindVBO() const {glBindBuffer(GL_ARRAY_BUFFER, vbo);}
    void bindEBO() const {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);}
    void unbind() const {glBindVertexArray(0);}

    void draw() const {
        if (index_count == UINT32_MAX) glDrawArrays(draw_mode, 0, 3);
        else glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, nullptr);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
        vao = 0;
        vbo = 0;
        ebo = 0;
    }
};

struct Light {
    glm::vec3 pos;
    glm::vec3 color;
};

struct RenderInfo {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    std::vector<Light> lights;
};

// -------------------------
// !---- Simple Shader ----!

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

// -------------------------
// !---- Simple Shader ----!

struct FrameBuffer {
    GLuint fbo;
    GLuint colorBuffer;
    GLuint depthBuffer;
    uint32_t width;
    uint32_t height;
};

struct AttribInfo {
    uint32_t index;
    size_t size;
    GLenum type;
    bool normalized = false;
    GLsizei stride;
    size_t offset;
};

struct VAOConfig {
    std::vector<AttribInfo> attributes;
    size_t size_vertex;
    size_t num_vertices;
    size_t index_count = UINT32_MAX;
    GLenum draw_mode = GL_TRIANGLES;
    GLenum usage = GL_DYNAMIC_DRAW;
};