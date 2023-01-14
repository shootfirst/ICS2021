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

size_t events_read(void *buf, size_t offset, size_t len) {
  //*********************************pa3***************************************
  AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
  if (AM_KEY_NONE == kbd.keycode) {
    return 0;
  }
  char res[64];
  memset(res, 0, 64);
  assert(sprintf(res, "Got  (kbd): %s (%d) %s\n", keyname[kbd.keycode], kbd.keycode, kbd.keydown ? "DOWN" : "UP") <= 64);
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

// size_t fb_write(const void *buf, size_t offset, size_t len) {
//   //*********************************pa3*************************************
//   uintptr_t *ptr;
//   ptr = (uintptr_t *)(&buf);

//   io_write(AM_GPU_MEMCPY, offset, (void *)*ptr, len);
//   io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  
//   return len;
//   //*********************************pa3*************************************
//   return 0;
  
// }

// void init_device() {
//   Log("Initializing devices...");
//   ioe_init();
// }


static AM_GPU_CONFIG_T gpu_config;
static AM_GPU_FBDRAW_T gpu_fbdraw;
#define NUM_LEN 32
static char *__itoa(int num, char *buff)
{
  char tmp[NUM_LEN];
  if (num == 0)
  {
    strcat(buff, "0");
    return buff;
  }

  uint8_t i = 0;
  while (num != 0)
  {
    tmp[i] = num % 10 + '0';
    num /= 10;
    i++;
  }

  for (int j = i - 1; j >= 0; --j)
    buff[i - 1 - j] = tmp[j];
  buff[i] = '\0';

  return buff;
}


size_t dispinfo_read(void *buf, size_t offset, size_t len)
{
  // yield();
  // printf("disp offset is %d\n", offset);
  // ioe_read(AM_GPU_CONFIG, &gpu_config);
  int width = gpu_config.width, height = gpu_config.height;

  char num_buf[NUM_LEN];
  // important: must strcpy! becuase buf may have some rubbish left!
  // strcat will remain them and past imformation behind....
  strcpy(buf, "WIDTH:");
  strcat(buf, __itoa(width, num_buf));
  strcat(buf, "\nHEIGHT:");
  strcat(buf, __itoa(height, num_buf));
  strcat(buf, "\n");
  return strlen((char *)buf);
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


size_t fb_write(const void *buf, size_t offset, size_t len)
{
  // yield();
  // printf("len is %d\n",len);
  if (len == 0)
  {
    // printf("end!\n");
    gpu_fbdraw.sync = 1;
    gpu_fbdraw.w = 0;
    gpu_fbdraw.h = 0;
    ioe_write(AM_GPU_FBDRAW, &gpu_fbdraw);
    return 0;
  }

  // ioe_read(AM_GPU_CONFIG, &gpu_config);
  int width = gpu_config.width;
  // printf("screen is %d\n", width);

  gpu_fbdraw.pixels = (void *)buf;
  gpu_fbdraw.w = len;
  gpu_fbdraw.h = 1;
  gpu_fbdraw.x = offset % width;
  gpu_fbdraw.y = offset / width;
  // printf("w,h is %d,%d\n",gpu_fbdraw.w,gpu_fbdraw.h);
  gpu_fbdraw.sync = 0;
  ioe_write(AM_GPU_FBDRAW, &gpu_fbdraw);
  return len;
}

void init_device()
{
  Log("Initializing devices...");
  ioe_init();

  ioe_read(AM_GPU_CONFIG, &gpu_config);
}
