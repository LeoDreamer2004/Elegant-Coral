#ifndef CORAL_H
#define CORAL_H

#include <glm/ext/scalar_constants.hpp>
#include <vector>

#include "glad/glad.h"

struct Vertex {
    GLfloat x, y, z; // position
    GLfloat r, g, b; // color
};

struct Triangle {
    GLushort a, b, c; // index
};

class CoralRenderer;
class Coral {
    friend class CoralRenderer;

    float smin;
    float smax;
    float v;
    float sub;
    float br;
    float theta;

    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;

public:
    explicit Coral(float smin = 0, float smax = 1, float v = 0.2, float sub = 0.3, float br = 0.4,
                   float theta = glm::pi<float>() / 3);
};

class CoralRenderer {
    unsigned int VAO, VBO, EBO;
    const Coral &coral;

public:
    explicit CoralRenderer(const Coral &coral);

    ~CoralRenderer();

    void render() const;
};

#endif // CORAL_H
