#pragma once

#include <memory>
#include <vector>
#include "hierro/utils/color.h"
#include "hierro/shader.h"
#include "hierro/component/component.h"

class Block: public Component {
public:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  Shader shader;

  float radius = 1.0f;

  Color color = Color::rgb(0.5, 0.5, 0.5);

  Block();

  // impl Component
  float width = 0.25;
  float height = 0.25;
  float x = 0.5 - width / 2;
  float y = 0.5 + height / 2;
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  virtual void draw() override;
  virtual std::pair<float*, float*> get_position() override;
  virtual std::pair<float*, float*> get_size() override;
  virtual std::vector<std::unique_ptr<Component>>* get_children() override;
  virtual Component** get_father() override;

private:
  void update_vertices();
  void update_indices();
  void init_shader();
};
