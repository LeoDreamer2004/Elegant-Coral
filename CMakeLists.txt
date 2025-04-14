cmake_minimum_required(VERSION 3.10)
project(P05_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(PkgConfig REQUIRED)


set(GLAD_SOURCES src/glad/glad.c)
add_executable(P05_3 ${GLAD_SOURCES} src/camera.cpp src/shader.cpp src/main.cpp)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/glsl
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/glsl ${CMAKE_CURRENT_BINARY_DIR}/glsl
        COMMENT "Copying glsl directory to ${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/glsl
)

add_custom_target(CopyGlsl_P05_3 ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/glsl
)

add_dependencies(P05_3 CopyGlsl_P05_3)

target_link_libraries(P05_3 glfw FreeGLUT::freeglut ${OPENGL_LIBRARIES})
