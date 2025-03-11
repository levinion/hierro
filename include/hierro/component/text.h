#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "freetype/freetype.h"
#include "hierro/shader.h"
#include "hierro/color.h"

class Character {
public:
  unsigned int texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class TextGenerater {
public:
  static TextGenerater* get_instance();
  void init(std::string font);
  void draw_text(std::string text, float x, float y, float scale, Color color);
  void viewport(float x, float y);

private:
  static TextGenerater* instance;
  std::unordered_map<char32_t, Character> character_table;
  Shader shader;
  void init_freetype(std::string font);
  void init_shader();
  void init_buffer();
  void add_character(char32_t c);
  unsigned int vao;
  unsigned int vbo;
  FT_Library ft;
  FT_Face face;
};
