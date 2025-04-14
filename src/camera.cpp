#include "camera.hpp"

void Camera::updateAxis() {
    right = normalize(cross(front, worldUp));
    up = normalize(cross(front, right));
}

void Camera::processScroll(float yoffset) {
    fov -= yoffset;
    if (fov < 10.0f) fov = 10.0f;
    if (fov > 120.0f) fov = 120.0f;
}

#include <iostream>

void Camera::processMouse(float xoffset, float yoffset) {
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // 球面坐标系转换为笛卡尔坐标系
    front = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );
}

void Camera::move(CameraMovement direction, float deltaTime) {
    float distance = realSpeed * deltaTime;
    glm::vec3 worldFront = front;
    worldFront.y = 0;
    worldFront = normalize(worldFront);
    switch (direction) {
        case FORWARD:
            position += worldFront * distance;
            break;
        case BACKWARD:
            position -= worldFront * distance;
            break;
        // 沿着右轴移动
        case LEFT:
            position -= right * distance;
            break;
        case RIGHT:
            position += right * distance;
            break;
        // 沿着上轴移动
        case UP:
            position += worldUp * distance;
            break;
        case DOWN:
            position -= worldUp * distance;
            break;
    }
}

void Camera::setSpeedUp(bool speedUp) {
    realSpeed = movementSpeed * (speedUp ? 1.5f : 1.0f);
    realFov = fov * (speedUp ? 1.05f : 1.0f);
}
