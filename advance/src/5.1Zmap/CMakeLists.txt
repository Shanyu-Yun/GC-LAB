cmake_minimum_required(VERSION 3.15)
project(Z-Map)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VCPKG_ROOT C:/enviroment/vcpkg/)

set(CMAKE_TOOLCHAIN_FILE C:/enviroment/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_PREFIX_PATH C:/enviroment/vcpkg/installed/x64-windows/share)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)

file(GLOB SANDBOX_SRC tool.hpp tool.cpp camera.hpp camera.cpp shader.hpp shader.cpp Zmap.cpp Zmap.hpp sandbox.cpp)

add_executable(advance-5.1 ${SANDBOX_SRC})
target_link_libraries(advance-5.1 glfw glad::glad)

# 配置文件，用于存储资源文件路径
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)
include_directories(${CMAKE_BINARY_DIR})