#include <glad/glad.h>
#include <GL/gl.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <codecvt>
#include <glm/glm.hpp>
#include "hierro/app.h"
#include "hierro/shader/text/vertex.h"
#include "hierro/shader/text/fragment.h"
#include "hierro/component/text.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

TextGenerater* TextGenerater::instance = nullptr;

TextGenerater* TextGenerater::get_instance() {
  if (TextGenerater::instance == nullptr) {
    TextGenerater::instance = new TextGenerater();
  }
  return TextGenerater::instance;
}

void TextGenerater::init(std::string font) {
  this->init_freetype(font);
  this->init_shader();
  this->init_buffer();
}

void TextGenerater::init_freetype(std::string font) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  this->face = face;
  this->ft = ft;

  FT_Set_Pixel_Sizes(this->face, 0, 48);

  FT_Select_Charmap(this->face, ft_encoding_unicode);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

  for (char32_t c = 0; c < 128; c++) {
    this->add_character(c);
    // FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);
  }

  // FT_Done_Face(face);
  // FT_Done_FreeType(ft);
}

void TextGenerater::init_shader() {
  auto vertex = (const char*)_text_vertex_shader_code;
  auto fragment = (const char*)_text_fragment_shader_code;
  auto shader = Shader(vertex, fragment);
  this->shader = shader;
  auto app = Application::get_instance();
  auto size = app->window_size();
  glm::mat4 projection = glm::ortho(
    0.0f,
    static_cast<float>(size.first),
    0.0f,
    static_cast<float>(size.second)
  );
  shader.use();
  glUniformMatrix4fv(
    glGetUniformLocation(shader.id(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
}

void TextGenerater::init_buffer() {
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
}

void TextGenerater::draw_text(
  std::string text,
  float x,
  float y,
  float scale,
  Color color
) {
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

  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
  auto s = utf32conv.from_bytes(text);

  // iterate through all characters
  for (auto c = s.begin(); c != s.end(); c++) {
    if (!this->character_table.contains(*c)) {
      this->add_character(*c);
    }

    auto ch = this->character_table[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
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
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.advance >> 6)
      * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
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

void TextGenerater::add_character(char32_t c) {
  if (FT_Load_Char(this->face, c, FT_LOAD_RENDER)) {
    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    return;
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
}
