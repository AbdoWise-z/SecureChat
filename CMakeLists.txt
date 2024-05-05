cmake_minimum_required(VERSION 3.28)
project(SecureChat)

set(CMAKE_CXX_STANDARD 17)

set(IMGUI_LOCATION "C:/Users/xAbdoMo/CLionProjects/imgui")

include_directories(${IMGUI_LOCATION} "${IMGUI_LOCATION}/backends")

set(IMGUI_SOURCE_FILES
        ${IMGUI_LOCATION}/imgui.cpp
        ${IMGUI_LOCATION}/imgui_draw.cpp
        ${IMGUI_LOCATION}/imgui_demo.cpp
        ${IMGUI_LOCATION}/imgui_tables.cpp
        ${IMGUI_LOCATION}/imgui_widgets.cpp
)

add_library(imgui ${IMGUI_SOURCE_FILES})

set(IMGUI_SOURCE_BACKEND
        ${IMGUI_LOCATION}/backends/imgui_impl_dx12.cpp
        ${IMGUI_LOCATION}/backends/imgui_impl_win32.cpp
)
add_library(imgui_backend ${IMGUI_SOURCE_BACKEND})


add_executable(SecureChat
        main.cpp
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

target_link_libraries(
        SecureChat
        imgui
        imgui_backend
        -lgmp
        -ld3d12
        -ldxgi
        -ld3dcompiler
        -ldwmapi
        -lWs2_32
)


add_executable(
        test
        test.cpp
        util/BigInt.cpp
        sec/ElGamalSignature.cpp
        sec/sha1.cpp
        sec/sha1.hpp
        sec/SHA256.cpp
        sec/SHA256.h
        sec/aes256.cpp
        sec/aes256.hpp
        util/BinaryInt.cpp
        util/BinaryInt.h
)

target_link_libraries(
        test
        -lgmp
)
