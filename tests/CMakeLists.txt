set(name "hierro_example")
cmake_minimum_required(VERSION 3.10)
project(${name} CXX)

# clang setting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_CXX_STANDARD 26)

# src
# include_directories(include)
file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${name} ${SRC})

# link std library static
target_link_options(${name} PRIVATE -static-libgcc -static-libstdc++)

add_subdirectory(.. hierro)
target_link_libraries(${name} hierro)
#
# third party
# add_subdirectory(../third_party/glfw-3.4 tests/glfw)
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# target_link_libraries(${name} glfw)
#
# find_package(OpenGL REQUIRED)
# target_link_libraries(${name} OpenGL::GL)
#
# add_subdirectory(../third_party/glad tests/glad)
# target_link_libraries(${name} glad)
