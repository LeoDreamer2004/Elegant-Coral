#include "window.h"

#include <iostream>

Window::Window() = default;

Window::~Window() {
    delete renderer;
    delete coral;
    delete shader;
    delete camera;

    glfwTerminate();
}

bool Window::init() {
    // prepare the window
    if (!glfwInit()) {
        return false;
    }
    window = glfwCreateWindow(800, 600, "Elegant Coral", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // init the glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // set callback functions
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); });
    glfwSetCursorPosCallback(window, [](GLFWwindow *w, double x, double y) {
        auto self = static_cast<Window *>(glfwGetWindowUserPointer(w));
        self->processMouse(x, y);
    });
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // member initialization
    shader = new Shader(Resources("coral_v.glsl"), Resources("coral_f.glsl"));
    coral = new Coral();
    renderer = new CoralRenderer(*coral);
    camera = new Camera();

    // window config
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    return true;
}

void Window::processKeyboard() {
    static bool EscLastPressed = false;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !EscLastPressed) {
        paused = !paused;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        if (!paused)
            atFirst = true;
    }
    EscLastPressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera->setSpeedUp(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->move(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->move(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->move(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->move(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->move(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->move(DOWN, deltaTime);
}

void Window::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        // process input
        processKeyboard();

        // clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // apply shader
        shader->use();

        // set GL matrices
        camera->updateAxis();
        auto model = glm::mat4(1.0f);
        auto view = camera->getViewMatrix();
        auto projection = camera->getProjectionMatrix(800.f / 600);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);

        // draw coral
        renderer->render();

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::processMouse(double xPos, double yPos) {
    static float lastX, lastY;
    auto x = static_cast<float>(xPos), y = static_cast<float>(yPos);
    if (paused)
        return;
    if (atFirst) {
        lastX = x, lastY = y;
        atFirst = false;
    }
    camera->processMouse(x - lastX, lastY - y);
    lastX = x, lastY = y;
}

void Window::processScroll(double, double yOffset) const {
    camera->processScroll(static_cast<float>(yOffset));
}
