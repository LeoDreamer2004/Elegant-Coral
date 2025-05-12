#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "camera.h"
#include "coral.h"
#include "shader.h"

class Window {
    bool paused = false;
    bool atFirst = true;

    GLFWwindow *window = nullptr;
    Coral *coral = nullptr;
    CoralRenderer *renderer = nullptr;
    Shader *shader = nullptr;
    Camera *camera = nullptr;

    void processKeyboard();

    void processMouse(double xPos, double yPos);

    void processScroll(double xOffset, double yOffset) const;

public:
    Window();

    ~Window();

    bool init();

    void mainLoop();
};

#endif // WINDOW_H
