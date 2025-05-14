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
    static bool EnterLastPressed = false;

    auto pressed = [this](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };

    if (pressed(GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    } else if (pressed(GLFW_KEY_ESCAPE) && !EscLastPressed) {
        paused = !paused;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        if (!paused)
            atFirst = true;
    }
    EscLastPressed = pressed(GLFW_KEY_ESCAPE);

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera->setSpeedUp(pressed(GLFW_KEY_LEFT_CONTROL) || pressed(GLFW_KEY_RIGHT_CONTROL));

    if (pressed(GLFW_KEY_W) || pressed(GLFW_KEY_UP))
        camera->move(FORWARD, deltaTime);
    if (pressed(GLFW_KEY_S) || pressed(GLFW_KEY_DOWN))
        camera->move(BACKWARD, deltaTime);
    if (pressed(GLFW_KEY_A) || pressed(GLFW_KEY_LEFT))
        camera->move(LEFT, deltaTime);
    if (pressed(GLFW_KEY_D) || pressed(GLFW_KEY_RIGHT))
        camera->move(RIGHT, deltaTime);
    if (pressed(GLFW_KEY_SPACE))
        camera->move(UP, deltaTime);
    if (pressed(GLFW_KEY_RIGHT_SHIFT) || pressed(GLFW_KEY_LEFT_SHIFT))
        camera->move(DOWN, deltaTime);
    if (pressed(GLFW_KEY_ENTER) && !EnterLastPressed) {
        coral->grow();
        renderer->updateBuffers();
    }

    EnterLastPressed = pressed(GLFW_KEY_ENTER);
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
