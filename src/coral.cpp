#include "coral.h"

#include <cstddef>
#include <cstring>

#include "glad/glad.h"

#include <iostream>

Coral::Coral(float smin, float smax, float v, float sub, float br, float theta)
    : alpha(0.1), smin(smin), smax(smax), v(v), sub(sub), br(br), theta(theta) {
    // start from a tetrahedron
    vertices = {{{0.0f, -0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
                {{-0.1f, -0.1f, 0.1f}, {0.0f, 1.0f, 0.0f}},
                {{0.1f, -0.1f, 0.1f}, {0.0f, 0.0f, 1.0f}},
                {{0.0f, 0.15f, 0.0f}, {1.0f, 1.0f, 0.0f}}};
    // DO NOT CHANGE THE ORDER! (clockwise)
    triangles = {{0, 1, 2}, {1, 3, 2}, {2, 3, 0}, {3, 1, 0}};
    trMap = {{0, {0, 1, 3}}, {1, {0, 1, 3}}, {2, {0, 1, 2}}, {3, {1, 2, 3}}};
}

glm::vec3 Coral::triangleNormal(const Triangle &tr) const {
    const auto &[a, color1] = vertices[tr.a];
    const auto &[b, color2] = vertices[tr.b];
    const auto &[c, color3] = vertices[tr.c];

    // n = ab x ac
    return -glm::normalize(glm::cross(b - a, c - a)); // the normal is to outside
};

void Coral::grow() { return surfaceGrowth(); }

void Coral::surfaceGrowth() {
    // choose a random vertice to grow
    GLushort v = rand() % vertices.size();

    // use the average of triangles' normals as the normal of v
    // FIXME: This is not OK.
    glm::vec3 n{};
    for (auto &i : trMap[v]) {
        const auto &triangle = triangles[i];
        n += triangleNormal(triangle);
    }

    n = glm::normalize(n);
    vertices[v].pos += n * alpha;
}

void Coral::colonyBranching() {}

void Coral::polypCloning() {}

const std::vector<Vertex> &Coral::getVertices() const { return vertices; }

const std::vector<Triangle> &Coral::getTriangles() const { return triangles; }

CoralRenderer::CoralRenderer(const Coral &coral) : VAO(0), VBO(0), EBO(0), coral(coral) {
    // bind VAO, VBO & EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(coral.getVertices().size() * sizeof(Vertex)),
                 coral.getVertices().data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<long>(coral.getTriangles().size() * sizeof(Triangle)),
                 coral.getTriangles().data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, color)));
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

void CoralRenderer::updateBuffers() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    static_cast<long>(coral.getVertices().size() * sizeof(Vertex)),
                    coral.getVertices().data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    static_cast<long>(coral.getTriangles().size() * sizeof(Triangle)),
                    coral.getTriangles().data());
}