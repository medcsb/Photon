#include "Renderer.hpp"

#include <utility>
#include <stdexcept>

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::initFrameBuffer(uint32_t width, uint32_t height) {
    m_mainFrame = createFrameBuffer(width, height);
}

void Renderer::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_mainFrame.fbo);
    glViewport(0, 0, m_mainFrame.width, m_mainFrame.height);

    // rendering code
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_simpleRenderSystem.render(m_renderInfo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer Renderer::createFrameBuffer(uint32_t width, uint32_t height) {
    FrameBuffer fb;
    fb.width = width;
    fb.height = height;
    glGenFramebuffers(1, &fb.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);

    glGenTextures(1, &fb.colorBuffer);
    glBindTexture(GL_TEXTURE_2D, fb.colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.colorBuffer, 0);

    glGenRenderbuffers(1, &fb.depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fb.depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.depthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fb;
}