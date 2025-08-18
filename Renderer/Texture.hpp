#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include <string>

enum class TexUsage : uint8_t {
    ALBEDO,
    NORMAL,
    METALLIC,
    ROUGHNESS,
};

class Texture {
private:
    GLuint m_id = 0;
    GLint m_unit = 0;
public:
    Texture();
    ~Texture();

    GLuint getTextureId() const { return m_id; }
    GLint getTextureUnit() const { return m_unit; }
    void loadTexture(const std::string& path, GLint unit = 0, bool genMipMaps = true);
};