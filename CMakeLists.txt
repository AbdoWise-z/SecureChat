cmake_minimum_required(VERSION 3.28)
project(SecureChat)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)           # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)     # Add variables to use High Performance Graphics Card if available
add_subdirectory(vendor/glfw)                       # Build the GLFW project to use later as a library

set(GLAD_SOURCE
        vendor/glad/src/gl.c
)

set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_impl/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl/imgui_impl_opengl3.cpp
        vendor/imgui/imgui_impl/imgui_impl_win32.cpp
)

set(VENDOR_SOURCES ${GLAD_SOURCE} ${IMGUI_SOURCES})

set(APP_SOURCE
        sec/ElGamalSignature.cpp
        util/BigInt.cpp
        sec/sha1.cpp
        sec/sha1.hpp
        util/BinaryInt.cpp
        util/BinaryInt.h
        UI.cpp
        UI.h
        Chat.cpp
        Chat.h
        sec/SHA256.cpp
        sec/SHA256.h
        sec/aes256.cpp
        sec/aes256.hpp
)

include_directories(
        source/common
        vendor/glfw/include
        vendor/glad/include
        vendor/imgui
        vendor/imgui/imgui_impl
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

add_executable(SecureChat main.cpp ${APP_SOURCE} ${VENDOR_SOURCES})

target_link_libraries(
        SecureChat
        glfw
        dwmapi
        Ws2_32
        gmp
        opengl32
)


