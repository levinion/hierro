#pragma once

#include <cassert>
#include <functional>
#include <vector>
#include "hierro/utils/data.hpp"

namespace hierro {

class Component {
public:
  // render api
  virtual void draw() = 0;
  // layout api
  virtual Position& get_position() = 0;
  virtual Size& get_size() = 0;
  virtual std::vector<Component*>& get_children() = 0;
  virtual Component*& get_father() = 0;
  // hook api
  virtual std::function<void(int, int, int)>& get_click_callback() = 0;
  virtual std::function<void(int, int, int, int)>& get_key_callback() = 0;
  virtual std::function<void(unsigned int)>& get_input_callback() = 0;
  virtual std::function<void()>& get_focus_callback() = 0;

  virtual ~Component() = default;

  virtual Position absolute_position() {
    auto father = this->get_father();
    assert(father != nullptr);
    auto father_pos = father->absolute_position();
    auto father_size = father->absolute_size();
    auto self_pos = this->get_position();
    return { father_pos.x + father_size.width * self_pos.x,
             father_pos.y - father_size.height * (1.0 - self_pos.y) };
  }

  virtual Size absolute_size() {
    auto father = this->get_father();
    assert(father != nullptr);
    auto father_size = father->absolute_size();
    auto size_scale = this->get_size();
    return { size_scale.width * father_size.width,
             size_scale.height * father_size.height };
  }

  virtual void center() {
    auto size = this->get_size();
    auto& pos = this->get_position();
    pos.x = 0.5 - size.width / 2;
    pos.y = 0.5 + size.height / 2;
  }

  virtual void set_position(float x, float y) {
    auto& pos = this->get_position();
    pos.x = x;
    pos.y = y;
  }

  virtual void set_size(float width, float height) {
    auto& size = this->get_size();
    size.width = width;
    size.height = height;
  }

  virtual Component* add_child(Component* child) {
    auto& father = child->get_father();
    assert(father == nullptr);
    father = this;
    assert(father != nullptr);
    auto& children = this->get_children();
    children.push_back(child);
    return this;
  }

  virtual void draw_children() {
    auto& children = this->get_children();
    for (auto& child : children) {
      child->draw();
      child->draw_children();
    }
  }

  // button, action, mods
  virtual void on_click(std::function<void(int, int, int)> callback) {
    auto& click_callback = this->get_click_callback();
    click_callback = callback;
  }

  virtual void send_click_event(int button, int action, int mods) {
    this->get_click_callback()(button, action, mods);
  }

  // @param int key
  // @param int scancode
  // @param int action
  // @param int mod
  virtual void on_key(std::function<void(int, int, int, int)> callback) {
    auto& key_callback = this->get_key_callback();
    key_callback = callback;
  }

  virtual void send_key_event(int key, int scancode, int action, int mod) {
    this->get_key_callback()(key, scancode, action, mod);
  }

  // param unsigned int codepoint
  virtual void on_input(std::function<void(unsigned int)> callback) {
    auto& input_callback = this->get_input_callback();
    input_callback = callback;
  }

  virtual void send_input_event(unsigned int codepoint) {
    this->get_input_callback()(codepoint);
  }

  // do not set this hook with on_click at the same time
  virtual void on_focus(std::function<void()> callback) {
    auto& focus_callback = this->get_focus_callback();
    focus_callback = callback;
  }

  virtual void send_focus_event() {
    this->get_focus_callback()();
  }

  virtual bool is_hitted(float x, float y) {
    auto [px, py] = this->absolute_position();
    auto [width, height] = this->absolute_size();
    return (x >= px && x <= px + width && y >= py - height && y <= py);
  }
};

#define GET_SET(type, attr) \
  type get_##attr() { \
    return this->attr; \
  } \
  void set_##attr(type attr) { \
    this->attr = attr; \
  }

#define GET_REF(ret, c, name) \
  ret& c::get_##name() { \
    return this->name; \
  }

#define IMPL_COMPONENT(T) \
  GET_REF(Position, T, position) \
  GET_REF(Size, T, size) \
  GET_REF(std::vector<Component*>, T, children) \
  GET_REF(Component*, T, father) \
  GET_REF(std::function<void(int, int, int)>, T, click_callback) \
  GET_REF(std::function<void(int, int, int, int)>, T, key_callback) \
  GET_REF(std::function<void(unsigned int)>, T, input_callback) \
  GET_REF(std::function<void()>, T, focus_callback)

} // namespace hierro
