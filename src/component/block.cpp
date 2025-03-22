#include <glad/glad.h>
#include "hierro/component/block.h"
#include "hierro/app.h"
#include "hierro/shader/block/vertex.h"
#include "hierro/shader/block/fragment.h"
#include "hierro/shader.h"

Block::Block() {
  // init vao
  glGenVertexArrays(1, &this->vao);
  // init vbo
  glGenBuffers(1, &this->vbo);
  // init ebo
  glGenBuffers(1, &this->ebo);

  this->init_shader();

  // init vertices and indices
  this->update_vertices();
  this->update_indices();

  // update layout
  // position: x, y
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

void Block::draw() {
  glBindVertexArray(this->vao);
  this->shader.use();
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

  auto size = Application::get_instance()->window_size();
  glUniform2f(
    glGetUniformLocation(this->shader.id(), "size"),
    this->width * size.first,
    this->height * size.second
  );

  glUniform2f(
    glGetUniformLocation(this->shader.id(), "position"),
    this->x * size.first,
    this->y * size.second
  );
}

void Block::update_vertices() {
  float x = (this->x - 0.5) * 2;
  float y = (this->y - 0.5) * 2;
  float height = this->height * 2;
  float width = this->width * 2;
  this->vertices = {
    // left down
    x,
    y - height,
    // right down
    x + width,
    y - height,
    // left up
    x,
    y,
    // right up
    x + width,
    y,
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

void Block::update(std::function<void(Block*)>&& f) {
  f(this);
  this->update_vertices();
}

void Block::set_position(float x, float y) {
  this->x = x;
  this->y = y;
}

void Block::center() {
  this->set_position(0.5 - width / 2, 0.5 + height / 2);
}

void Block::init_shader() {
  auto vertex_shader_code = (const char*)_block_vertex_shader_code;
  auto fragment_shader_code = (const char*)_block_fragment_shader_code;

  this->shader = Shader(vertex_shader_code, fragment_shader_code);
}
