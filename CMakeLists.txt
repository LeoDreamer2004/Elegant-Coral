cmake_minimum_required(VERSION 3.10)
project(ElegantCoral)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(PkgConfig REQUIRED)

add_executable(ElegantCoral
        src/glad/glad.c
        src/util/resources.cpp
        src/camera.cpp
        src/shader.cpp
        src/main.cpp
)

# copy sources
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        COMMENT "Copying resource directory to ${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

add_custom_target(CopyResource ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(ElegantCoral CopyResource)
target_link_libraries(ElegantCoral glfw FreeGLUT::freeglut ${OPENGL_LIBRARIES})
