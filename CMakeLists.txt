# project
set(name "hierro")
cmake_minimum_required(VERSION 3.10)
project(${name} CXX)

# clang setting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)

# src
file(GLOB_RECURSE SRC src/*.cpp)
add_library(${name} ${SRC})
target_include_directories(${name} PUBLIC include/)

# link std library static
target_link_options(${name} PRIVATE -static-libgcc -static-libstdc++)

# third party
option(ENABLE_SDL_BACKEND "enable hierro's sdl backend" ON)
option(ENABLE_GLFW_BACKEND "enable hierro's glfw backend" OFF)
option(ENABLE_VIDEO_COMPONENT "enable hierro's video component" OFF)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(${name} PUBLIC OpenGL::GL)

add_subdirectory(./vendor/glad)
target_link_libraries(${name} PUBLIC glad)

add_subdirectory(./vendor/stb_image)
target_link_libraries(${name} PUBLIC stb_image)

add_subdirectory(./vendor/magic_enum)
target_link_libraries(${name} PUBLIC magic_enum)

find_package(PkgConfig REQUIRED)

pkg_check_modules(Freetype2 REQUIRED freetype2)
target_link_libraries(${name} PUBLIC ${Freetype2_LIBRARIES})
target_include_directories(${name} PUBLIC ${Freetype2_INCLUDE_DIRS})

pkg_check_modules(spdlog REQUIRED spdlog)
target_link_libraries(${name} PRIVATE ${spdlog_LIBRARIES})
target_include_directories(${name} PUBLIC ${spdlog_INCLUDE_DIRS})

if(ENABLE_SDL_BACKEND)
  target_compile_definitions(${name} PUBLIC ENABLE_SDL_BACKEND)
  pkg_check_modules(SDL3 REQUIRED sdl3)
  target_link_libraries(${name} PUBLIC ${SDL3_LIBRARIES})
  target_include_directories(${name} PUBLIC ${SDL3_INCLUDE_DIRS})
endif()

if(ENABLE_GLFW_BACKEND)
  target_compile_definitions(${name} PUBLIC ENABLE_GLFW_BACKEND)
  pkg_check_modules(GLFW3 REQUIRED glfw3)
  target_link_libraries(${name} PUBLIC ${GLFW3_LIBRARIES})
  target_include_directories(${name} PUBLIC ${GLFW3_INCLUDE_DIRS})
endif()

if(ENABLE_VIDEO_COMPONENT)
  target_compile_definitions(${name} PUBLIC ENABLE_VIDEO_COMPONENT)
  pkg_check_modules(Mpv REQUIRED mpv)
  target_link_libraries(${name} PUBLIC ${Mpv_LIBRARIES})
  target_include_directories(${name} PUBLIC ${Mpv_INCLUDE_DIRS})
  target_include_directories(${name} PUBLIC ./vendor/concurrentqueue)
endif()
