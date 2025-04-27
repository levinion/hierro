#pragma once

#include <memory>
#include <vector>
#include "hierro/shader/shader.hpp"
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"

namespace hierro {

class Block: public Component {
public:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  Shader shader;

  float radius = 0.0f;
  Color color = Color(0.5, 0.5, 0.5);
  float border_thickness = 0.0f;
  Color border_color = Color(1.0, 0.0, 1.0);

  void set_texture(char* pixels, int width, int height);
  void free_texture();

  Block();
  ~Block() = default;

  // impl Component
  Size size = { 0.25f, 0.25f };
  Position position = { 0.5 - size.width / 2, 0.5 + size.height / 2 };
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  COMPONENT_OVERRIDE_METHODS

  // custom api
  void load_custom_shader(const char* shader_source);

private:
  void update_vertices();
  void update_indices();
  void init_shader();

  // texture
  bool texture_enabled = false;
  unsigned int texture;
  unsigned int placehold_texture;

  COMPONENT_DEFAULT_CALLBACK
};

} // namespace hierro
