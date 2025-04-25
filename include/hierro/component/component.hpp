#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <vector>
#include "hierro/utils/data.hpp"
#include "hierro/event/event.hpp"

namespace hierro {

class Component {
public:
  // render api
  virtual void draw() = 0;
  // layout api
  virtual Position& get_position() = 0;
  virtual Size& get_size() = 0;
  virtual std::vector<std::unique_ptr<Component>>& get_children() = 0;
  virtual Component*& get_father() = 0;
  // hook api
  virtual std::function<void(ClickEvent)>& get_click_callback() = 0;
  virtual std::function<void(KeyEvent)>& get_key_callback() = 0;
  virtual std::function<void(unsigned int)>& get_input_callback() = 0;
  virtual std::function<void()>& get_focus_callback() = 0;
  virtual std::function<void(MouseMoveEvent)>& get_mouse_move_callback() = 0;
  virtual std::function<void(MouseWheelEvent)>& get_mouse_wheel_callback() = 0;

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

  template<typename T>
  T* add_child() {
    static_assert(
      std::is_base_of<Component, T>::value,
      "T should impl hierro::Component"
    );
    auto child = std::make_unique<T>();
    auto& father = child->get_father();
    assert(father == nullptr);
    father = this;
    assert(father != nullptr);
    auto& children = this->get_children();
    auto cp = child.get();
    children.push_back(std::move(child));
    return cp;
  }

  virtual void draw_children() {
    auto& children = this->get_children();
    for (auto& child : children) {
      child->draw();
      child->draw_children();
    }
  }

  virtual Component* on_click(std::function<void(ClickEvent e)> callback) {
    auto& click_callback = this->get_click_callback();
    click_callback = callback;
    return this;
  }

  virtual void send_click_event(ClickEvent e) {
    this->get_click_callback()(e);
  }

  virtual Component* on_key(std::function<void(KeyEvent e)> callback) {
    auto& key_callback = this->get_key_callback();
    key_callback = callback;
    return this;
  }

  virtual void send_key_event(KeyEvent e) {
    this->get_key_callback()(e);
  }

  virtual Component* on_input(std::function<void(unsigned int)> callback) {
    auto& input_callback = this->get_input_callback();
    input_callback = callback;
    return this;
  }

  virtual void send_input_event(unsigned int codepoint) {
    this->get_input_callback()(codepoint);
  }

  // do not set this hook with on_click at the same time
  virtual Component* on_focus(std::function<void()> callback) {
    auto& focus_callback = this->get_focus_callback();
    focus_callback = callback;
    return this;
  }

  virtual void send_focus_event() {
    this->get_focus_callback()();
  }

  virtual Component*
  on_mouse_move(std::function<void(MouseMoveEvent e)> callback) {
    auto& mouse_move_callback = this->get_mouse_move_callback();
    mouse_move_callback = callback;
    return this;
  }

  virtual void send_mouse_move_event(MouseMoveEvent e) {
    this->get_mouse_move_callback()(e);
  }

  virtual Component*
  on_mouse_wheel(std::function<void(MouseWheelEvent e)> callback) {
    auto& mouse_wheel_callback = this->get_mouse_wheel_callback();
    mouse_wheel_callback = callback;
    return this;
  }

  virtual void send_mouse_wheel_event(MouseWheelEvent e) {
    this->get_mouse_wheel_callback()(e);
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
  GET_REF(std::vector<std::unique_ptr<Component>>, T, children) \
  GET_REF(Component*, T, father) \
  GET_REF(std::function<void(ClickEvent)>, T, click_callback) \
  GET_REF(std::function<void(KeyEvent)>, T, key_callback) \
  GET_REF(std::function<void(MouseMoveEvent)>, T, mouse_move_callback) \
  GET_REF(std::function<void(MouseWheelEvent)>, T, mouse_wheel_callback) \
  GET_REF(std::function<void(unsigned int)>, T, input_callback) \
  GET_REF(std::function<void()>, T, focus_callback)

#define COMPONENT_DEFAULT_CALLBACK \
  std::function<void(ClickEvent)> click_callback = [](ClickEvent) {}; \
  std::function<void(KeyEvent)> key_callback = [](KeyEvent) {}; \
  std::function<void(unsigned int)> input_callback = [](unsigned int) {}; \
  std::function<void()> focus_callback = [] {}; \
  std::function<void(MouseMoveEvent)> mouse_move_callback = \
    [](MouseMoveEvent) {}; \
  std::function<void(MouseWheelEvent)> mouse_wheel_callback = \
    [](MouseWheelEvent) {};

#define COMPONENT_OVERRIDE_METHODS \
  virtual void draw() override; \
  virtual Position& get_position() override; \
  virtual Size& get_size() override; \
  virtual std::vector<std::unique_ptr<Component>>& get_children() override; \
  virtual Component*& get_father() override; \
  virtual std::function<void(ClickEvent)>& get_click_callback() override; \
  virtual std::function<void(KeyEvent)>& get_key_callback() override; \
  virtual std::function<void(unsigned int)>& get_input_callback() override; \
  virtual std::function<void()>& get_focus_callback() override; \
  virtual std::function<void(MouseMoveEvent)>& get_mouse_move_callback() \
    override; \
  virtual std::function<void(MouseWheelEvent)>& get_mouse_wheel_callback() \
    override;

} // namespace hierro
