#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

enum class RenderType : uint8_t {
    Simple = 0,
    PBR = 1
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