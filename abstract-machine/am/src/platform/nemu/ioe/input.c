#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //***********************************pa2********************************
  // kbd->keydown = 0;
  // kbd->keycode = AM_KEY_NONE;
  uint32_t keyboard_code = inl(KBD_ADDR);
  kbd->keydown = (keyboard_code & KEYDOWN_MASK ? true : false);
  kbd->keycode = (~KEYDOWN_MASK) & keyboard_code;
  //***********************************pa2********************************
}
