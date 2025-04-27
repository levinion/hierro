init:
  cmake -B build

init_windows:
  cmake -B build -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake

build:
  cmake --build build -j 8

embed:
  - mkdir -p ./include/hierro/shader/block
  - mkdir -p ./include/hierro/shader/text
  xxd -n _block_vertex_shader_code -i ./assets/shader/block/block.vert | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/block/vertex.hpp
  xxd -n _block_fragment_shader_code -i ./assets/shader/block/block.frag | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/block/fragment.hpp
  xxd -n _text_vertex_shader_code -i ./assets/shader/text/text.vert | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/text/vertex.hpp
  xxd -n _text_fragment_shader_code -i ./assets/shader/text/text.frag | sed "s/}/,\'\\\0\'}/" > ./include/hierro/shader/text/fragment.hpp
  clang-format -i ./include/hierro/shader/**/*.hpp

clean:
  rm -rf build

tokei:
  tokei -e third_party
