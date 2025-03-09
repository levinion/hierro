#include <functional>
#include <vector>
#include "color.h"

class Block {
public:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  unsigned int program;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

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
  void init_program();
};
