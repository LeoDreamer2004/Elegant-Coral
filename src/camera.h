#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
    float sensitivity = 0.05f;
    float movementSpeed = 2.5f;
    float realSpeed = movementSpeed;
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

public:
    float yaw = -90.0f;
    float pitch = 0.0f;

    float fov = 45.0f;
    float realFov = fov;

    void processMouse(float xOffset, float yOffset);

    void processScroll(float yOffset);

    void move(CameraMovement direction, float deltaTime);

    void updateAxis();

    void setSpeedUp(bool speedUp);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(float aspect) const;
};

#endif // CAMERA_H
