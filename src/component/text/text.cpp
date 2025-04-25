#include <glad/glad.h>
#include <GL/gl.h>
#include <expected>
#include <glm/glm.hpp>
#include <freetype2/freetype/freetype.h>
#include <freetype2/ft2build.h>
#include "hierro/app.hpp"
#include "hierro/error.hpp"
#include "hierro/shader/text/vertex.hpp"
#include "hierro/shader/text/fragment.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/component/text.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include FT_FREETYPE_H

namespace hierro {

TextGenerater* TextGenerater::instance = nullptr;

TextGenerater* TextGenerater::get_instance() {
  if (TextGenerater::instance == nullptr) {
    TextGenerater::instance = new TextGenerater();
  }
  return TextGenerater::instance;
}

std::expected<void, std::string>
TextGenerater::init(std::string font, Size size) {
  this->font_size = size;
  hierro_check(this->init_freetype(font));
  hierro_check(this->init_shader());
  hierro_check(this->init_buffer());
  return {};
}

// TODO: this should receive a font list
HierroResult<void> TextGenerater::init_freetype(std::string font) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    return hierro_err("ERROR::FREETYPE: Could not init FreeType Library");

  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face))
    return hierro_err("ERROR::FREETYPE: Failed to load font");

  this->face = face;
  this->ft = ft;

  FT_Set_Pixel_Sizes(this->face, this->font_size.width, this->font_size.height);

  FT_Select_Charmap(this->face, ft_encoding_unicode);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

  for (char32_t c = 0; c < 128; c++) {
    if (auto r = this->add_character(c); !r)
      return r;
    // FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);
  }
  return {};
}

HierroResult<void> TextGenerater::init_shader() {
  auto vertex = (const char*)_text_vertex_shader_code;
  auto fragment = (const char*)_text_fragment_shader_code;
  auto shader = Shader("text_shader", vertex, fragment);
  this->shader = shader;
  auto app = Application::get_instance();
  auto size = app->window_size();
  glm::mat4 projection = glm::ortho(
    0.0f,
    static_cast<float>(size.width),
    0.0f,
    static_cast<float>(size.height)
  );
  shader.use();
  glUniformMatrix4fv(
    glGetUniformLocation(shader.id(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
  return {};
}

HierroResult<void> TextGenerater::init_buffer() {
  // init vao
  glGenVertexArrays(1, &this->vao);
  // init vbo
  glGenBuffers(1, &this->vbo);
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return {};
}

HierroResult<void> TextGenerater::draw_text(
  std::wstring text,
  Position position,
  Size size,
  bool wrap,
  bool overflow,
  float spacing,
  float line_spacing,
  float scale,
  Color color,
  VerticalAlign vertical_align,
  HorizontalAlign horizontal_align
) {
  bool wrap_flag = false;

  // calculate max size of render box
  double max_width = 0.0, max_height = this->line_height();
  {
    // x,y is abosulte positon
    auto x = position.x;
    auto y = position.y;

    for (auto c = text.begin(); c != text.end(); c++) {
      if (wrap_flag) {
        if (x - position.x > max_width) {
          max_width = x - position.x;
        }
        x = position.x;
        y -= this->line_height() * line_spacing;
        max_height += this->line_height() * line_spacing;
        wrap_flag = false;
      }

      // handle '\n'
      if (*c == '\n') {
        wrap_flag = true;
        continue;
      }

      if (!this->character_table.contains(*c)) {
        hierro_check(this->add_character(*c));
      }

      auto ch = this->character_table[*c];

      float xpos = x + ch.bearing.x * scale;
      float ypos = y - this->line_height() - (ch.size.y - ch.bearing.y) * scale;

      float w = ch.size.x * scale;
      float h = ch.size.y * scale;

      if (xpos + w > position.x + size.width) {
        if (wrap) {
          wrap_flag = true;
          continue;
        } else {
          break;
        }
      }

      if (!overflow && ypos < position.y - size.height) {
        break;
      }

      x += (ch.advance / 64.0f) * spacing * scale;
    }
    if (x - position.x > max_width) {
      max_width = x - position.x;
    }
  }

  // set offset based on align options
  switch (vertical_align) {
    case VerticalAlign::Center:
      position.x += (size.width - max_width) / 2;
      break;
    case VerticalAlign::Left:
      break;
    case VerticalAlign::Right:
      position.x += size.width - max_width;
      break;
  }

  switch (horizontal_align) {
    case HorizontalAlign::Center:
      position.y -= (size.height - max_height) / 2;
      break;
    case HorizontalAlign::Top:
      break;
    case HorizontalAlign::Bottom:
      position.y -= size.height - max_height;
      break;
  }

  // activate corresponding render state
  this->shader.use();
  glUniform3f(
    glGetUniformLocation(this->shader.id(), "textColor"),
    color.r,
    color.g,
    color.b
  );
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);

  // iterate through all characters

  auto x = position.x;
  auto y = position.y;

  wrap_flag = false;

  for (auto c = text.begin(); c != text.end(); c++) {
    // wrap line
    if (wrap_flag) {
      x = position.x;
      y -= this->line_height() * line_spacing;
      wrap_flag = false;
    }

    // handle '\n'
    if (*c == '\n') {
      wrap_flag = true;
      continue;
    }

    if (!this->character_table.contains(*c)) {
      hierro_check(this->add_character(*c));
    }

    auto ch = this->character_table[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - this->line_height() - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    if (xpos + w > position.x + size.width) {
      if (wrap) {
        wrap_flag = true;
        continue;
      } else {
        break;
      }
    }

    if (!overflow && ypos < position.y - size.height) {
      break;
    }

    // update VBO for each character
    float vertices[6][4] = { { xpos, ypos + h, 0.0f, 0.0f },
                             { xpos, ypos, 0.0f, 1.0f },
                             { xpos + w, ypos, 1.0f, 1.0f },

                             { xpos, ypos + h, 0.0f, 0.0f },
                             { xpos + w, ypos, 1.0f, 1.0f },
                             { xpos + w, ypos + h, 1.0f, 0.0f } };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.advance / 64.0f) * spacing * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  return {};
}

void TextGenerater::viewport(float x, float y) {
  glm::mat4 projection =
    glm::ortho(0.0f, static_cast<float>(x), 0.0f, static_cast<float>(y));
  shader.use();
  glUniformMatrix4fv(
    glGetUniformLocation(shader.id(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
}

std::expected<void, std::string> TextGenerater::add_character(char32_t c) {
  if (FT_Load_Char(this->face, c, FT_LOAD_RENDER)) {
    return hierro_err("failed to load glyph");
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    face->glyph->bitmap.width,
    face->glyph->bitmap.rows,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    face->glyph->bitmap.buffer
  );
  // set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // now store character for later use
  Character character = {
    texture,
    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    (unsigned int)face->glyph->advance.x
  };
  this->character_table.insert(std::pair<char32_t, Character>(c, character));
  return {};
}

void TextGenerater::destroy() {
  FT_Done_Face(this->face);
  FT_Done_FreeType(this->ft);
}

float TextGenerater::line_height() {
  return this->font_size.height;
}

} // namespace hierro
