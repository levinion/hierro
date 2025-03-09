#include <glad/glad.h>
#include <cstddef>
#include "hierro/component/block.h"
#include "hierro/shader/vertex.h"
#include "hierro/shader/fragment.h"

Block::Block() {
  // init vao
  glGenVertexArrays(1, &this->vao);
  // init vbo
  glGenBuffers(1, &this->vbo);
  // init ebo
  glGenBuffers(1, &this->ebo);

  this->init_program();

  // init vertices and indices
  this->update_vertices();
  this->update_indices();

  // update layout
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    1,
    4,
    GL_FLOAT,
    GL_FALSE,
    7 * sizeof(float),
    (void*)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);
}

void Block::draw() {
  glBindVertexArray(this->vao);
  glUseProgram(this->program);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Block::update_vertices() {
  this->vertices = {
    this->x,         this->y - height, 0.0f, // left down
    this->color.r,   this->color.g,    this->color.b, this->color.a, //

    this->x + width, this->y - height, 0.0f, // right down
    this->color.r,   this->color.g,    this->color.b, this->color.a, //

    this->x,         this->y,          0.0f, // left up
    this->color.r,   this->color.g,    this->color.b, this->color.a, //

    this->x + width, this->y,          0.0f, // right up
    this->color.r,   this->color.g,    this->color.b, this->color.a //
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
  this->set_position(-width / 2, height / 2);
}

void Block::init_program() {
  auto vertex_shader_code = (const char*)_vertex_shader_code;
  auto fragment_shader_code = (const char*)_fragment_shader_code;

  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
  glCompileShader(vertex_shader);

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
  glCompileShader(fragment_shader);

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  this->program = shader_program;

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}
