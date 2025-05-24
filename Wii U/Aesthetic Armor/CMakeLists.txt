cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("./PluginSystem.cmake" REQUIRED)

project(lightarmor CXX)
add_compile_options(-Ofast -Wall)

add_executable(lightarmor main.cpp)

find_library(LIBKERNEL kernel REQUIRED HINTS "${DEVKITPRO}/wums/lib")
target_include_directories(lightarmor PRIVATE "${DEVKITPRO}/wums/include")
target_link_libraries(lightarmor "${LIBKERNEL}")
target_link_options(lightarmor PRIVATE "-T${DEVKITPRO}/wums/share/libkernel.ld")

wups_create_plugin(lightarmor)
