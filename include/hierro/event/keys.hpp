#pragma once

#pragma once

namespace hierro {
enum class Key : int {
  Unknown = 0, // UNKNOWN

  A = 4,
  B = 5,
  C = 6,
  D = 7,
  E = 8,
  F = 9,
  G = 10,
  H = 11,
  I = 12,
  J = 13,
  K = 14,
  L = 15,
  M = 16,
  N = 17,
  O = 18,
  P = 19,
  Q = 20,
  R = 21,
  S = 22,
  T = 23,
  U = 24,
  V = 25,
  W = 26,
  X = 27,
  Y = 28,
  Z = 29,

  Num1 = 30,
  Num2 = 31,
  Num3 = 32,
  Num4 = 33,
  Num5 = 34,
  Num6 = 35,
  Num7 = 36,
  Num8 = 37,
  Num9 = 38,
  Num0 = 39,

  Enter = 40, // Enter
  Escape = 41, // Esc
  Backspace = 42,
  Tab = 43,
  Space = 44,

  Minus = 45, // -
  Equals = 46, // =
  LeftBracket = 47, // [
  RightBracket = 48, // ]
  Backslash = 49, // \

  Semicolon = 51, // ;
  Apostrophe = 52, // '
  Grave = 53, // `
  Comma = 54, // ,
  Period = 55, // .
  Slash = 56, // /

  CapsLock = 57,

  // F1–F12
  F1 = 58,
  F2 = 59,
  F3 = 60,
  F4 = 61,
  F5 = 62,
  F6 = 63,
  F7 = 64,
  F8 = 65,
  F9 = 66,
  F10 = 67,
  F11 = 68,
  F12 = 69,

  PrintScreen = 70,
  ScrollLock = 71,
  Pause = 72,

  Insert = 73,
  Home = 74,
  PageUp = 75,
  Delete = 76,
  End = 77,
  PageDown = 78,

  ArrowRight = 79,
  ArrowLeft = 80,
  ArrowDown = 81,
  ArrowUp = 82,

  // Pad
  NumLockClear = 83, // NumLock
  KP_Divide = 84,
  KP_Multiply = 85,
  KP_Minus = 86,
  KP_Plus = 87,
  KP_Enter = 88,
  KP_1 = 89,
  KP_2 = 90,
  KP_3 = 91,
  KP_4 = 92,
  KP_5 = 93,
  KP_6 = 94,
  KP_7 = 95,
  KP_8 = 96,
  KP_9 = 97,
  KP_0 = 98,
  KP_Period = 99,

  // Mod
  LCtrl = 224,
  LShift = 225,
  LAlt = 226,
  LGUI = 227,
  RCtrl = 228,
  RShift = 229,
  RAlt = 230,
  RGUI = 231,

  Count = 512
};

enum class MouseButton {
  Unknown = -1, // 未知按键
  Left = 1, // BUTTON1
  Right = 2, // BUTTON2
  Middle = 3, // BUTTON3
  X1 = 4, // BUTTON4
  X2 = 5, // BUTTON5
  Button6 = 6,
  Button7 = 7,
  Button8 = 8
};
} // namespace hierro
