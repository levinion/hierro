#include <glad/glad.h>
#include "hierro/component/block.hpp"
#include "hierro/app.hpp"
#include "hierro/component/component.hpp"
#include "hierro/shader/block/vertex.hpp"
#include "hierro/shader/block/fragment.hpp"
#include "hierro/shader.hpp"

namespace hierro {

Block::Block() {
  // init vao
  glGenVertexArrays(1, &this->vao);
  // init vbo
  glGenBuffers(1, &this->vbo);
  // init ebo
  glGenBuffers(1, &this->ebo);

  this->init_shader();

  // indices
  this->update_indices();

  // update layout
  // position: x, y
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    4 * sizeof(float),
    (void*)(2 * sizeof(float))
  );
  glEnableVertexAttribArray(1);
}

void Block::draw() {
  // send vertices before drawing
  this->update_vertices();
  this->shader.use();

  if (this->texture_enabled) {
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // update uniforms
  glUniform1f(glGetUniformLocation(this->shader.id(), "radius"), this->radius);
  glUniform4f(
    glGetUniformLocation(this->shader.id(), "color"),
    this->color.r,
    this->color.g,
    this->color.b,
    this->color.a
  );

  // size of block
  auto size = this->absolute_size();
  glUniform2f(
    glGetUniformLocation(this->shader.id(), "size"),
    size.width,
    size.height
  );

  // position of left-upper vertex
  auto position = this->absolute_position();
  glUniform2f(
    glGetUniformLocation(this->shader.id(), "position"),
    position.x,
    position.y
  );

  glUniform1f(
    glGetUniformLocation(this->shader.id(), "border_thickness"),
    this->border_thickness
  );

  glUniform4f(
    glGetUniformLocation(this->shader.id(), "border_color"),
    this->border_color.r,
    this->border_color.g,
    this->border_color.b,
    this->border_color.a
  );
}

void Block::update_vertices() {
  auto window_size = Application::get_instance()->window_size();
  auto absolute_position = this->absolute_position();
  auto absolute_size = this->absolute_size();

  // 0~1 -> -1~1
  float x = absolute_position.x / window_size.width * 2 - 1;
  float y = absolute_position.y / window_size.height * 2 - 1;
  // 0~1 -> 0~2
  float width = absolute_size.width / window_size.width * 2;
  float height = absolute_size.height / window_size.height * 2;

  this->vertices = { // left down
                     x,
                     y - height,
                     0.0,
                     0.0,
                     // right down
                     x + width,
                     y - height,
                     1.0,
                     0.0,
                     // left up
                     x,
                     y,
                     0.0,
                     1.0,
                     // right up
                     x + width,
                     y,
                     1.0,
                     1.0
  };

  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(
    GL_ARRAY_BUFFER,
    this->vertices.size() * sizeof(float),
    this->vertices.data(),
    GL_STATIC_DRAW
  );
}

void Block::update_indices() {
  this->indices = {
    0, 1, 3, //
    0, 2, 3,
  };
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    this->indices.size() * sizeof(unsigned int),
    this->indices.data(),
    GL_STATIC_DRAW
  );
}

void Block::init_shader() {
  auto vertex_shader_code = (const char*)_block_vertex_shader_code;
  auto fragment_shader_code = (const char*)_block_fragment_shader_code;

  this->shader = Shader(vertex_shader_code, fragment_shader_code);
}

void Block::set_texture(cv::Mat& image) {
  if (this->texture_enabled)
    return;
  cv::flip(image, image, 0);
  unsigned int texture;
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, image.step / image.elemSize());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    image.cols,
    image.rows,
    0,
    GL_BGR,
    GL_UNSIGNED_BYTE,
    image.ptr()
  );
  glGenerateMipmap(GL_TEXTURE_2D);
  this->texture_enabled = true;
  this->texture = texture;

  glUniform1i(
    glGetUniformLocation(this->shader.id(), "texture_enabled"),
    this->texture_enabled
  );
}

void Block::free_texture() {
  if (this->texture_enabled) {
    glDeleteTextures(1, &this->texture);
    this->texture = 0;
    this->texture_enabled = false;
  }
}

IMPL_COMPONENT(Block)

} // namespace hierro
