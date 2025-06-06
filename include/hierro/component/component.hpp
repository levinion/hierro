#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <vector>
#include "hierro/utils/error.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/event/event.hpp"

namespace hierro {

class Component {
public:
  // render api
  virtual HierroResult<void> draw() = 0;
  // layout api
  virtual Position& get_position() = 0;
  virtual Size& get_size() = 0;
  virtual std::vector<std::unique_ptr<Component>>& get_children() = 0;
  virtual Component*& get_father() = 0;
  // hook api
  virtual std::function<void(ClickEvent)>& get_click_callback() = 0;
  virtual std::function<void(KeyEvent)>& get_key_callback() = 0;
  virtual std::function<void(InputEvent)>& get_input_callback() = 0;
  virtual std::function<void(FocusEvent)>& get_focus_callback() = 0;
  virtual std::function<void(UnFocusEvent)>& get_unfocus_callback() = 0;
  virtual std::function<void(MouseMoveEvent)>& get_mouse_move_callback() = 0;
  virtual std::function<void(MouseWheelEvent)>& get_mouse_wheel_callback() = 0;

  // TODO: give every component a dirty property, showing it should be redraw on the next frame
  virtual bool is_dirty() {
    return true;
  };

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

  template<typename T, typename... Args>
  auto add_child(Args... args) {
    static_assert(
      std::is_base_of<Component, T>::value,
      "T should impl hierro::Component"
    );
    auto child = std::make_unique<T>();
    // set child's father field to this component
    auto& father = child->get_father();
    father = this;
    // add child to this component's children vector
    auto& children = this->get_children();
    auto p = child.get();
    children.push_back(std::move(child));
    // init child
    return p->init(std::forward<Args>(args)...);
  }

  virtual HierroResult<void> draw_children() {
    auto& children = this->get_children();
    for (auto& child : children) {
      hierro_check(child->draw());
      hierro_check(child->draw_children());
    }
    return {};
  }

  virtual Component* on_click(std::function<void(ClickEvent e)> callback) {
    auto& click_callback = this->get_click_callback();
    click_callback = callback;
    return this;
  }

  virtual void emit_click_event(ClickEvent e) {
    this->get_click_callback()(e);
  }

  virtual Component* on_key(std::function<void(KeyEvent e)> callback) {
    auto& key_callback = this->get_key_callback();
    key_callback = callback;
    return this;
  }

  virtual void emit_key_event(KeyEvent e) {
    this->get_key_callback()(e);
  }

  virtual Component* on_input(std::function<void(InputEvent)> callback) {
    auto& input_callback = this->get_input_callback();
    input_callback = callback;
    return this;
  }

  virtual void emit_input_event(InputEvent e) {
    this->get_input_callback()(e);
  }

  // do not set this hook with on_click at the same time
  virtual Component* on_focus(std::function<void(FocusEvent)> callback) {
    auto& focus_callback = this->get_focus_callback();
    focus_callback = callback;
    return this;
  }

  virtual void emit_focus_event(FocusEvent e) {
    this->get_focus_callback()(e);
  }

  virtual Component* on_unfocus(std::function<void(UnFocusEvent)> callback) {
    auto& unfocus_callback = this->get_unfocus_callback();
    unfocus_callback = callback;
    return this;
  }

  virtual void emit_unfocus_event(UnFocusEvent e) {
    this->get_unfocus_callback()(e);
  }

  virtual Component*
  on_mouse_move(std::function<void(MouseMoveEvent e)> callback) {
    auto& mouse_move_callback = this->get_mouse_move_callback();
    mouse_move_callback = callback;
    return this;
  }

  virtual void emit_mouse_move_event(MouseMoveEvent e) {
    this->get_mouse_move_callback()(e);
  }

  virtual Component*
  on_mouse_wheel(std::function<void(MouseWheelEvent e)> callback) {
    auto& mouse_wheel_callback = this->get_mouse_wheel_callback();
    mouse_wheel_callback = callback;
    return this;
  }

  virtual void emit_mouse_wheel_event(MouseWheelEvent e) {
    this->get_mouse_wheel_callback()(e);
  }

  virtual bool is_hitted(float x, float y) {
    auto [px, py] = this->absolute_position();
    auto [width, height] = this->absolute_size();
    return (x >= px && x <= px + width && y >= py - height && y <= py);
  }

  // A simple way to bind all event callbacks to another component, which is useful when creating widgets
  void set_proxy(Component* proxy) {
    this->on_input([=](InputEvent e) { proxy->emit_input_event(e); });
    this->on_focus([=](FocusEvent e) { proxy->emit_focus_event(e); });
    this->on_unfocus([=](UnFocusEvent e) { proxy->emit_unfocus_event(e); });
    this->on_key([=](KeyEvent e) { proxy->emit_key_event(e); });
    this->on_click([=](ClickEvent e) { proxy->emit_click_event(e); });
    this->on_mouse_move([=](MouseMoveEvent e) {
      proxy->emit_mouse_move_event(e);
    });
    this->on_mouse_wheel([=](MouseWheelEvent e) {
      proxy->emit_mouse_wheel_event(e);
    });
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
  GET_REF(std::function<void(InputEvent)>, T, input_callback) \
  GET_REF(std::function<void(FocusEvent)>, T, focus_callback) \
  GET_REF(std::function<void(UnFocusEvent)>, T, unfocus_callback)

#define COMPONENT_DEFAULT_CALLBACK \
  std::function<void(ClickEvent)> click_callback = [](ClickEvent) {}; \
  std::function<void(KeyEvent)> key_callback = [](KeyEvent) {}; \
  std::function<void(InputEvent)> input_callback = [](InputEvent) {}; \
  std::function<void(FocusEvent)> focus_callback = [](FocusEvent) {}; \
  std::function<void(UnFocusEvent)> unfocus_callback = [](UnFocusEvent) {}; \
  std::function<void(MouseMoveEvent)> mouse_move_callback = \
    [](MouseMoveEvent) {}; \
  std::function<void(MouseWheelEvent)> mouse_wheel_callback = \
    [](MouseWheelEvent) {};

#define COMPONENT_OVERRIDE_METHODS \
  virtual HierroResult<void> draw() override; \
  virtual Position& get_position() override; \
  virtual Size& get_size() override; \
  virtual std::vector<std::unique_ptr<Component>>& get_children() override; \
  virtual Component*& get_father() override; \
  virtual std::function<void(ClickEvent)>& get_click_callback() override; \
  virtual std::function<void(KeyEvent)>& get_key_callback() override; \
  virtual std::function<void(InputEvent)>& get_input_callback() override; \
  virtual std::function<void(FocusEvent)>& get_focus_callback() override; \
  virtual std::function<void(UnFocusEvent)>& get_unfocus_callback() override; \
  virtual std::function<void(MouseMoveEvent)>& get_mouse_move_callback() \
    override; \
  virtual std::function<void(MouseWheelEvent)>& get_mouse_wheel_callback() \
    override;

} // namespace hierro
