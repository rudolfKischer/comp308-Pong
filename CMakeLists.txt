cmake_minimum_required(VERSION 3.24)
project(COMP308_Pong)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)
add_executable(COMP308_Pong main.cpp)
find_package(glfw3 3.3.8 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(COMP308_Pong OpenGL::GL glfw)