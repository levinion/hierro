init:
  cmake -B build

init_windows:
  cmake -B build -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake

build:
  cmake --build build

embed:
  - mkdir -p ./include/hierro/shader/block 
  - mkdir -p ./include/hierro/shader/text 
  xxd -n _block_vertex_shader_code -i ./assets/shader/block/vertex.glsl | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/block/vertex.h 
  xxd -n _block_fragment_shader_code -i ./assets/shader/block/fragment.glsl | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/block/fragment.h 
  xxd -n _text_vertex_shader_code -i ./assets/shader/text/vertex.glsl | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/text/vertex.h 
  xxd -n _text_fragment_shader_code -i ./assets/shader/text/fragment.glsl | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/text/fragment.h 
  clang-format -i ./include/hierro/shader/**/*.h

clean:
  rm -rf build
