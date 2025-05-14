#ifndef CORAL_H
#define CORAL_H

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "glad/glad.h"

#include <unordered_map>
#include <unordered_set>

struct Vertex {
    glm::vec3 pos;   // x, y, z
    glm::vec3 color; // r, g, b
};

struct Triangle {
    GLushort a, b, c; // clockwise index
};

class Coral {

    /* Growth parameters */
    float alpha; // growing speed
    float smin;  // s_min
    float smax;  // s_max
    float v;     // elongation rate
    float sub;   // subdivision distance
    float br;    // inter-branching length
    float theta; // branching angle

    /* OpenGL status */
    std::vector<Vertex> vertices;    // all vertices in the coral
    std::vector<Triangle> triangles; // all triangles in the coral

    std::unordered_map<GLushort, std::unordered_set<GLushort>>
        trMap; // vertex -> the triangles' index it in

    glm::vec3 triangleNormal(const Triangle &tr) const;

public:
    explicit Coral(float smin = 0, float smax = 1, float v = 0.2, float sub = 0.3, float br = 0.4,
                   float theta = glm::pi<float>() / 3);

    void grow();

    void surfaceGrowth();
    void polypCloning();
    void colonyBranching();

    const std::vector<Vertex> &getVertices() const;
    const std::vector<Triangle> &getTriangles() const;
};

class CoralRenderer {
    unsigned int VAO, VBO, EBO;
    const Coral &coral;

public:
    explicit CoralRenderer(const Coral &coral);

    ~CoralRenderer();

    void render() const;

    void updateBuffers() const;
};

#endif // CORAL_H
