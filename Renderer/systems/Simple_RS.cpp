#include "Simple_RS.hpp"

#include <glm/gtc/type_ptr.hpp>

Simple_RS::Simple_RS() {}
Simple_RS::~Simple_RS() {}

void Simple_RS::add_toQueue(const SimpleRenderable& renderable) {
    m_renderableQueue.push_back(renderable);
}

void Simple_RS::flush() {
    m_renderableQueue.clear();
}

void Simple_RS::render(RenderInfo& renderInfo) {
    glUseProgram(m_shaderProgram);
    for (const auto& renderable : m_renderableQueue) {
        renderable.meshBuffer.bind();
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(renderable.transform));
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(renderInfo.viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(renderInfo.projectionMatrix));
        glUniform3fv(glGetUniformLocation(m_shaderProgram, "baseColor"), 1, glm::value_ptr(renderable.material.baseColor));
        glUniform1f(glGetUniformLocation(m_shaderProgram, "ambient"), renderable.material.ambient);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "diffuse"), renderable.material.diffuse);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "specular"), renderable.material.specular);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "specStrength"), renderable.material.specStrength);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "specPower"), renderable.material.specPower);
        glUniform3fv(glGetUniformLocation(m_shaderProgram, "lightPos"), 1, glm::value_ptr(renderInfo.lights[0].pos));
        glUniform3fv(glGetUniformLocation(m_shaderProgram, "lightColor"), 1, glm::value_ptr(renderInfo.lights[0].color));

        renderable.meshBuffer.draw();
    }
}