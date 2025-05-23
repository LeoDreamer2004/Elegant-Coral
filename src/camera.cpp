#include "camera.h"

void Camera::updateAxis() {
    right = normalize(cross(front, worldUp));
    up = normalize(cross(front, right));
}

void Camera::processScroll(float yOffset) {
    fov -= yOffset;
    if (fov < 10.0f)
        fov = 10.0f;
    if (fov > 120.0f)
        fov = 120.0f;
}

void Camera::processMouse(float xOffset, float yOffset) {
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    front = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
                      sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
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
    case LEFT:
        position -= right * distance;
        break;
    case RIGHT:
        position += right * distance;
        break;
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

glm::mat4 Camera::getViewMatrix() const { return glm::lookAt(position, position + front, worldUp); }

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(realFov), aspect, 0.1f, 100.0f);
}
