#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

class Component {
public:
  // render api
  virtual void draw() = 0;
  // layout api
  virtual std::pair<float*, float*> get_position() = 0;
  virtual std::pair<float*, float*> get_size() = 0;
  virtual std::vector<std::unique_ptr<Component>>* get_children() = 0;
  virtual Component** get_father() = 0;
  // hook api
  virtual std::function<void(int, int, int)>* get_click_callback() = 0;

  virtual ~Component() = default;

  virtual std::pair<float, float> absolute_position() {
    auto father = *this->get_father();
    assert(father != nullptr);
    auto [fp_x, fp_y] = father->absolute_position();
    auto [fs_x, fs_y] = father->absolute_size();
    auto [p_x, p_y] = this->get_position();
    return std::make_pair(fp_x + fs_x * *p_x, fp_y - fs_y * (1.0 - *p_y));
  }

  virtual std::pair<float, float> absolute_size() {
    auto father = *this->get_father();
    assert(father != nullptr);
    auto [fs_x, fs_y] = father->absolute_size();
    auto [width, height] = this->get_size();
    return std::make_pair(*width * fs_x, *height * fs_y);
  }

  virtual void center() {
    auto [width, height] = this->get_size();
    auto [x, y] = this->get_position();
    *x = 0.5 - *width / 2;
    *y = 0.5 + *height / 2;
  }

  virtual void set_position(float x, float y) {
    auto [px, py] = this->get_position();
    *px = x;
    *py = y;
  }

  virtual void set_size(float width, float height) {
    auto [sx, sy] = this->get_size();
    *sx = width;
    *sy = height;
  }

  virtual Component* add_child(Component* child) {
    auto father = child->get_father();
    assert(*father == nullptr);
    *father = this;
    assert(*father != nullptr);
    auto children = this->get_children();
    children->push_back(std::unique_ptr<Component> { child });
    return this;
  }

  virtual void draw_children() {
    auto children = this->get_children();
    for (auto& child : *children) {
      child->draw();
      child->draw_children();
    }
  }

  // button, action, mods
  virtual void on_click(std::function<void(int, int, int)> callback) {
    auto click_callback = this->get_click_callback();
    *click_callback = callback;
  }

  virtual bool is_hitted(float x, float y) {
    auto [px, py] = this->absolute_position();
    auto [width, height] = this->absolute_size();
    return (x >= px && x <= px + width && y >= py - height && y <= py);
  }
};

#define IMPL_COMPONENT(T) \
  std::pair<float*, float*> T::get_position() { \
    return std::make_pair(&this->x, &this->y); \
  } \
\
  std::pair<float*, float*> T::get_size() { \
    return std::make_pair(&this->width, &this->height); \
  } \
\
  std::vector<std::unique_ptr<Component>>* T::get_children() { \
    return &this->children; \
  } \
\
  Component** T::get_father() { \
    return &this->father; \
  } \
\
  std::function<void(int, int, int)>* T::get_click_callback() { \
    return &this->click_callback; \
  }
