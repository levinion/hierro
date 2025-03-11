#include <functional>
#include <vector>
#include "hierro/color.h"
#include "hierro/shader.h"

class Block {
public:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  Shader shader;

  float width = 0.5;
  float height = 0.5;
  float x = -width / 2;
  float y = height / 2;
  Color color = Color::rgb(0.5, 0.5, 0.5);

  Block();
  void update(std::function<void(Block*)>&& f);
  void draw();
  void set_position(float x, float y);
  void set_size(float x, float y);
  void center();

private:
  void update_vertices();
  void update_indices();
  void init_shader();
};
