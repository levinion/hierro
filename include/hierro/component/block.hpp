#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "hierro/shader.hpp"
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"

class Block: public Component {
public:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  Shader shader;

  float radius = 1.0f;
  Color color = Color(0.5, 0.5, 0.5);
  float border_thickness = 1.0f;
  Color border_color = Color(1.0, 0.0, 1.0);

  Block();

  // impl Component
  Size size = { 0.25f, 0.25f };
  Position position = { 0.5 - size.width / 2, 0.5 + size.height / 2 };
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  virtual void draw() override;
  virtual Position& get_position() override;
  virtual Size& get_size() override;
  virtual std::vector<std::unique_ptr<Component>>& get_children() override;
  virtual Component*& get_father() override;
  virtual std::function<void(int, int, int)>& get_click_callback() override;
  virtual std::function<void(int, int, int, int)>& get_key_callback() override;
  virtual std::function<void(unsigned int)>& get_input_callback() override;
  virtual std::function<void()>& get_focus_callback() override;

private:
  void update_vertices();
  void update_indices();
  void init_shader();

  std::function<void(int, int, int)> click_callback = [](int, int, int) {};
  std::function<void(int, int, int, int)> key_callback =
    [](int, int, int, int) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};
  std::function<void()> focus_callback = [] {};
};
