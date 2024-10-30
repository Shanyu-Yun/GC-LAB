cmake_minimum_required(VERSION 3.10)
project(OpenGLTest)

# Integrate with vcpkg
# Use environment variables or CMake options for portability
set(CMAKE_CXX_STANDARD 17)
set(VCPKG_ROOT C:/enviroment/vcpkg/)

set(CMAKE_TOOLCHAIN_FILE C:/enviroment/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_PREFIX_PATH C:/enviroment/vcpkg/installed/x64-windows/share)

# Finding required packages
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Shared utility library
file(GLOB SHARED_UTILS_SRC "src/utils/*.cpp" "src/utils/*.h")
add_library(utils ${SHARED_UTILS_SRC})
target_link_libraries(utils PRIVATE glad::glad glfw glm::glm assimp::assimp GLEW::GLEW )
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/utils)

# Levels and subprojects
set(LEVELS  medium) # ... add more levels as needed
set(MEDIUM_SUBPROJECTS 3.1objLoader 3.2lightWithModel 3.3gimbalLockCamera 3.4quaternionCamera 3.5framebuffer )


foreach(level IN LISTS LEVELS)
    message(STATUS "Level: ${level}")

    # Access the correct subproject list
    string(TOUPPER ${level} LEVEL_UPPERCASE)
    set(CURRENT_SUBPROJECTS ${${LEVEL_UPPERCASE}_SUBPROJECTS})

    foreach(subproject IN LISTS CURRENT_SUBPROJECTS)
        message(STATUS "  Subproject: ${subproject}")

        # Optional utility sources specific to subproject
        file(GLOB UTILS_SRC "src/${level}/${subproject}/utils/*.cpp" "src/${level}/${subproject}/utils/*.h")

        # Main executable
        add_executable(${subproject} "src/${level}/${subproject}/main.cpp" ${UTILS_SRC})
        target_link_libraries(${subproject} PRIVATE utils glad::glad glfw glm::glm assimp::assimp yaml-cpp::yaml-cpp)

        set_target_properties(${subproject} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${level}/${subproject}")

        # Copy dependencies to the output directory (if any)
        set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/${level}/${subproject}/dependencies")

        if(EXISTS ${SOURCE_DIR})
            add_custom_command(TARGET ${subproject} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${SOURCE_DIR} $<TARGET_FILE_DIR:${subproject}>
            )
        endif()
    endforeach()
endforeach()
