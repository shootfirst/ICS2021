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
//   char res[64];
//   memset(res, 0, 64);
//   assert(sprintf(res, "Got  (kbd): %s (%d) %s\n", keyname[kbd.keycode], kbd.keycode, kbd.keydown ? "DOWN" : "UP") <= 64);
//   size_t real_len = strlen(res);
//   strcpy(buf, res);
//   return real_len;

//   //*********************************pa3***************************************
// }
size_t events_read(void *buf, size_t offset, size_t len) {
  //*********************************pa3***************************************
  AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
  if (AM_KEY_NONE == kbd.keycode) {
    printf("11111111111111111111\n");
    return 0;
  }
  printf("22222222222222222222222\n");
  char res[64];
  memset(res, 0, 64);
  if (kbd.keydown) {
    assert(sprintf(res, "kd %s", keyname[kbd.keycode]) <= 64);
  } else {
    assert(sprintf(res, "ku %s", keyname[kbd.keycode]) <= 64);
  }
  size_t real_len = strlen(res);
  strcpy(buf, res);
  return real_len;
  //*********************************pa3***************************************
}

// size_t dispinfo_read(void *buf, size_t offset, size_t len) {
//   //*********************************pa3*************************************
//   AM_GPU_CONFIG_T gpu_config = io_read(AM_GPU_CONFIG);
//   int width = gpu_config.width;
//   int height = gpu_config.height;

//   char res[64];
//   memset(res, 0, 64);
//   // 格式： 宽度,高度
//   sprintf(res, "%d,%d", width, height);
//   size_t real_len = strlen(res);
//   if (len < real_len) {
//     return 0;
//   }

//   strcpy(buf, res);
//   return real_len;
//   //*********************************pa3*************************************
// }

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //*********************************pa3*************************************
  AM_GPU_CONFIG_T gpu_config = io_read(AM_GPU_CONFIG);
  int width = gpu_config.width;
  int height = gpu_config.height;

  char res[64];
  memset(res, 0, 64);
  // 格式： 宽度,高度
  sprintf(res, "WIDTH:%d\nHEIGHT:%d\n", width, height);
  size_t real_len = strlen(res);
  if (len < real_len) {
    return 0;
  }

  strcpy(buf, res);
  return real_len;
  //*********************************pa3*************************************
}


//*********************************pa3*************************************
#define END_LEN 0
static AM_GPU_CONFIG_T config; 
//*********************************pa3*************************************
size_t fb_write(const void *buf, size_t offset, size_t len) {
  AM_GPU_FBDRAW_T fbdraw;
  if (len == END_LEN) {
    fbdraw.sync = 1;
    fbdraw.w = 0;
    fbdraw.h = 0;
    ioe_write(AM_GPU_FBDRAW, &fbdraw);
    return 0;
  }
  int width = config.width;
  fbdraw.pixels = (void *)buf;
  fbdraw.w = len;
  fbdraw.h = 1;
  fbdraw.x = offset % width;
  fbdraw.y = offset / width;
  fbdraw.sync = 0;
  ioe_write(AM_GPU_FBDRAW, &fbdraw);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
  //******************************pa3*********************************
  ioe_read(AM_GPU_CONFIG, &config);
  //******************************pa3*********************************
}
