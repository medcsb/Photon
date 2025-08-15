#pragma once

#include "glad/glad.h"

#include <vector>

#include "RenderStructs.hpp"

class Buffer {
private:

public:
    static MeshBuffer createMeshBuffer(const VAOConfig& config, const void* vertex_data, const void* index_data = nullptr);

    static void deleteMeshBuffer(MeshBuffer& vao);

};