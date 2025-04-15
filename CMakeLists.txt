# project
set(name "hierro")
cmake_minimum_required(VERSION 3.10)
project(${name} CXX)

# clang setting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_CXX_STANDARD 26)

# src
file(GLOB_RECURSE SRC src/*.cpp)
add_library(${name} ${SRC})
target_include_directories(${name} PUBLIC include/)

# link std library static
target_link_options(${name} PRIVATE -static-libgcc -static-libstdc++)

# third party
add_subdirectory(./vendor/glfw-3.4)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
target_link_libraries(${name} PUBLIC glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${name} PUBLIC OpenGL::GL)

add_subdirectory(./vendor/glad)
target_link_libraries(${name} PUBLIC glad)

add_subdirectory(./vendor/freetype-2.13.3)
target_link_libraries(${name} PUBLIC freetype)

find_package(OpenCV REQUIRED)
target_link_libraries(${name} PUBLIC ${OpenCV_LIBRARIES})

target_include_directories(${name} PUBLIC ./vendor/glm/)
target_include_directories(${name} PUBLIC ./vendor/result-cpp/)
