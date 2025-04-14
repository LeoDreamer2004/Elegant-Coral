#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera  {
    float sensitivity = 0.05f;
    float movementSpeed = 2.5f;
    float realSpeed = movementSpeed;

public:
    // 摄像机属性
    glm::vec3 position {0.0f, 0.0f, 3.0f};
    glm::vec3 front {0.0f, 0.0f, -1.0f}; // 前向 (z轴)
    glm::vec3 up; // 上轴 (y轴)
    glm::vec3 right; // 右轴 (x轴)
    glm::vec3 worldUp {0.0f, 1.0f, 0.0f}; // 世界上向量

    // 欧拉角
    float yaw = -90.0f; // 偏航角
    float pitch = 0.0f; // 俯仰角
    // 我们不关心滚转角, 所以不需要

    // 视野
    float fov = 45.0f;
    float realFov = fov;

    void processMouse(float xoffset, float yoffset);
    void processScroll(float yoffset);
    void move(CameraMovement direction, float deltaTime);
    void updateAxis();
    void setSpeedUp(bool speedUp);
};

#endif
