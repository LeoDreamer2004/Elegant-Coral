#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "glad/glad.h"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    /* 读入文件代码 */

    string vertexStr;
    string fragmentStr;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // 保证 ifstream 对象可以抛出异常
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        // 读取文件到流
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();

        // 转换流到 string -> char *
        vertexStr = vShaderStream.str();
        fragmentStr = fShaderStream.str();
    } catch (ifstream::failure &e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << endl;
    }

    const char *vertexCode = vertexStr.c_str();
    const char *fragmentCode = fragmentStr.c_str();

    /* 编译着色器 */
    int success;
    char infoLog[512];
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
             << infoLog << endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
             << infoLog << endl;
    }

    /* 着色器程序 */
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // 检查链接错误
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
    }

    // 删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string &name, const glm::vec3 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(value));
}
