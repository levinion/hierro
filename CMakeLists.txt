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

find_package(OpenGL REQUIRED)
target_link_libraries(${name} PUBLIC OpenGL::GL)

add_subdirectory(./vendor/glad)
target_link_libraries(${name} PUBLIC glad)

find_package(Freetype REQUIRED)

find_package(SDL3 REQUIRED)
target_link_libraries(${name} PUBLIC ${SDL3_LIBRARIES})

find_package(PkgConfig REQUIRED)

pkg_check_modules(PLACEBO REQUIRED libplacebo)
target_link_libraries(${name} PUBLIC ${PLACEBO_LIBRARIES})

include_directories(/usr/include/freetype2)
pkg_check_modules(Freetype2 REQUIRED freetype2)
target_link_libraries(${name} PUBLIC ${Freetype2_LIBRARIES})

pkg_check_modules(GLFW3 REQUIRED glfw3)
target_link_libraries(${name} PUBLIC ${GLFW3_LIBRARIES})

target_include_directories(${name} PUBLIC ./vendor/glm/)
target_include_directories(${name} PUBLIC ./vendor/result/)
