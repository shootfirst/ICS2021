#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //**********************************pa3**************************************
  char *out = (char*)buf;
  for (int i = 0; i < len; ++i) {
    putch(out[i]);
  }
  return len;
  //**********************************pa3**************************************
}

// size_t events_read(void *buf, size_t offset, size_t len) {
//   //*********************************pa3***************************************
//   AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
//   if (AM_KEY_NONE == kbd.keycode) {
//     return 0;
//   }
//   printf("hehe\n");
//   char res[64];
//   memset(res, 0, 64);
//   assert(sprintf(res, "Got  (kbd): %s (%d) %s\n", keyname[kbd.keycode], kbd.keycode, kbd.keydown ? "DOWN" : "UP") <= 64);
//   size_t real_len = strlen(res);
//   strcpy(buf, res);
//   return real_len;

//   //*********************************pa3***************************************
// }

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) return 0;
  printf("yes here\n");
  int real_length = 4;
  char *tag = ev.keydown ? "kd " : "ku ";
  if (real_length <= len){
    strcpy(buf, tag);
  }else {
    assert(0);
    return 0;
  }
  real_length += strlen(keyname[ev.keycode]);
  
  if (real_length<= len){
    strcat(buf, keyname[ev.keycode]);
  }else {
    Log("Need %d for %s%s but got %d", strlen(keyname[ev.keycode]), buf, keyname[ev.keycode], len);
    assert(0);
    return 0;
  }
  Log("Got  (kbd): %s (%d) %s\n", keyname[ev.keycode], ev.keycode, ev.keydown ? "DOWN" : "UP");
  
  return real_length;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
