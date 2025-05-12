#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <string>

#include "util/resources.h"

class Shader {
public:
    unsigned int program;

    Shader(const Resources &vertexGL, const Resources &fragmentGL);

    ~Shader();

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setMat4(const std::string &name, const glm::mat4 &value) const;
};

#endif // SHADER_H
