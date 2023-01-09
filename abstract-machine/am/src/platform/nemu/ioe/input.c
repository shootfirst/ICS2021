#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //***********************************pa2********************************
  // kbd->keydown = 0;
  // kbd->keycode = AM_KEY_NONE;
  uint32_t keyboard_code = inl(KBD_ADDR);
  // 最高位是判断是按下还是弹回
  kbd->keydown = (keyboard_code & KEYDOWN_MASK ? true : false);
  // 其余位则表示按下的键
  kbd->keycode = (~KEYDOWN_MASK) & keyboard_code;
  //***********************************pa2********************************
}
