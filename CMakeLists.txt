cmake_minimum_required(VERSION 3.20)

project(Renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/minifb)

add_executable(${PROJECT_NAME} src/main.cpp
    src/application.h
    src/application.cpp
    src/window.cpp
    src/window.h
    src/renderer3D.h
    src/renderer3D.cpp
    src/rasterMath.cpp
    src/rasterMath.h
    src/mat4x4.h
    src/mat4x4.cpp
    src/vec3.h
    src/triangle.h
    src/input.h
    src/triangle.cpp
    src/vec3.cpp
    src/object.h
    src/object.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE minifb)
