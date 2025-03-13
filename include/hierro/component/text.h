#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "freetype/freetype.h"
#include "hierro/error.h"
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
  HierroResult<void> init(std::string font, unsigned int size);
  void draw_text(
    std::string text,
    std::pair<float, float> position,
    std::pair<float, float> size,
    float spacing,
    float line_spacing,
    float scale,
    Color color
  );
  void destroy();
  void viewport(float x, float y);
  float line_height();

private:
  static TextGenerater* instance;
  std::unordered_map<char32_t, Character> character_table;
  Shader shader;
  HierroResult<void> init_freetype(std::string font);
  HierroResult<void> init_shader();
  HierroResult<void> init_buffer();
  HierroResult<void> add_character(char32_t c);
  unsigned int vao;
  unsigned int vbo;
  FT_Library ft;
  FT_Face face;
  unsigned int font_base_size;
};
