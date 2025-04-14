#include "glad/glad.h"
#include "shader.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int SCR_WIDTH = 800.0;
const int SCR_HEIGHT = 600.0;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Camera camera;
bool paused = false;
bool atFirst = true;

// 鼠标移动回调函数
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    static float lastX, lastY;
    if (paused) return;
    if (atFirst) {
        lastX = xpos, lastY = ypos;
        atFirst = false;
    }
    camera.processMouse(xpos - lastX, lastY - ypos);
    lastX = xpos, lastY = ypos;
}

// 鼠标滚轮回调函数
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processScroll((float) yoffset);
}

void processInput(GLFWwindow *window) {
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

    static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    static float lastFrame = 0.0f; // 上一帧的时间

    // 按时间追踪, 防止不同设备上速度不一致
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.setSpeedUp(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.move(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.move(DOWN, deltaTime);
}

GLFWwindow *prepareWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
            glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyFirstWindow", NULL, NULL);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);

    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

// 前三位是顶点坐标, 中间三位是法向量, 后两位是纹理坐标
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
float texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};
glm::vec3 lightPosition(0.2f, 0.4f, 1.0f);

unsigned int genVBO() {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    return VBO;
}

unsigned int genVAO() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}

void configureVertexAttribute() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int loadTexture(const char *filePath, const int format, int textureUnit) {
    unsigned int texture;
    glGenTextures(1, &texture);

    // 激活指定的纹理单元
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图像数据
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << filePath << std::endl;
    }

    stbi_set_flip_vertically_on_load(true); // 翻转纹理防止上下颠倒
    stbi_image_free(data);

    return texture;
}

glm::vec3 lightColor(1.0f);

void renderCubes(const Shader &shader) {
    // 位置
    glm::vec3 cameraTarget = camera.position + camera.front;
    glm::mat4 view = lookAt(camera.position, cameraTarget, camera.worldUp);
    glm::mat4 projection = glm::perspective(glm::radians(camera.realFov),
                                            ((float) SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
    for (auto cubePosition: cubePositions) {
        glm::mat4 model(1.0f);
        model = translate(model, cubePosition);
        model = rotate(model, (float) glfwGetTime() * glm::radians(20.0f),
                       glm::vec3(0.5f, 0.3f, 0.2f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // 光照
    shader.setVec3("viewPos", camera.position);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);
    shader.setVec3("light.ambient", lightColor * 0.2f);
    shader.setVec3("light.diffuse", lightColor * 0.5f);
    shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // 镜面全力反射
    shader.setVec3("light.position", lightPosition);
    shader.setVec3("light.direction", camera.front);
    shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f))); // 传入余弦值, 方便比大小
    shader.setFloat("light.outerCutOff",glm::cos(glm::radians(20.0f)));
}

void renderLight(const Shader &shader) {
    glm::vec3 cameraTarget = camera.position + camera.front;
    glm::mat4 view = lookAt(camera.position, cameraTarget, camera.worldUp);
    glm::mat4 projection = glm::perspective(glm::radians(camera.realFov),
                                            ((float) SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 model(1.0f);
    model = translate(model, lightPosition);
    model = scale(model, glm::vec3(0.2f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setVec3("lightColor", lightColor);
}

int main() {
    GLFWwindow *window = prepareWindow();
    Shader cubeShader("./glsl/cube_vertex.glsl", "./glsl/cube_fragment.glsl");
    Shader lightShader("./glsl/light_vertex.glsl", "./glsl/light_fragment.glsl");

    unsigned int VBO = genVBO();
    unsigned int cubeVAO = genVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    configureVertexAttribute();

    unsigned int lightVAO = genVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    configureVertexAttribute();

    loadTexture("glsl/container.png", GL_RGBA, 0);
    loadTexture("glsl/container_specular.png", GL_RGBA, 1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updateAxis();

        glBindVertexArray(cubeVAO);
        cubeShader.use();

        renderCubes(cubeShader);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(lightVAO);
        lightShader.use();
        renderLight(lightShader);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
