#include "coral.h"

#include <cstddef>

#include "glad/glad.h"

Coral::Coral(float smin, float smax, float v, float sub, float br, float theta)
    : smin(smin), smax(smax), v(v), sub(sub), br(br), theta(theta) {
    vertices = {{0.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
                {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f},
                {0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f},
                {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f}};
    indices = {{0, 1, 2}, {0, 1, 3}, {1, 2, 3}, {2, 0, 3}};
}

CoralRenderer::CoralRenderer(const Coral &coral) : VAO(0), VBO(0), EBO(0), coral(coral) {
    // bind VAO, VBO & EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(coral.vertices.size() * sizeof(Vertex)),
                 coral.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<long>(coral.indices.size() * sizeof(Triangle)), coral.indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, x)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, r)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

CoralRenderer::~CoralRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CoralRenderer::render() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, nullptr);
}
