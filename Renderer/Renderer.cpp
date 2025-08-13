#include "Renderer.hpp"

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

void Renderer::add_toQueue(RenderInstance instance) {
    renderQueue.push_back(instance);
}

void Renderer::flush() {
    renderQueue.clear();
}

void Renderer::render() {
    float h;
}