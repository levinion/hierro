name := `cat build/CMakeCache.txt | grep CMAKE_PROJECT_NAME | awk -F '=' '{print $2}'` 

init:
  cmake -B build

init_windows:
  cmake -B build -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake

build:
  cmake --build build

embed:
  xxd -n _vertex_shader_code -i ./assets/shader/vertex.glsl | sed "s/}/,\'\\\0\'}/" > ./include/shader/vertex.h 
  xxd -n _fragment_shader_code -i ./assets/shader/fragment.glsl | sed "s/}/,\'\\\0\'}/" > ./include/shader/fragment.h 
  clang-format -i ./include/shader/*

run:
  just build
  ./build/{{name}}

run_windows:
  just build
  ./build/{{name}}.exe

clean:
  rm -rf build
