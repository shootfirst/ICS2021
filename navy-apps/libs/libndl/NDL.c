#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//********************************pa3**********************************
#include <sys/time.h>
#include <fcntl.h>
#include <assert.h>

#define IS_NUM(ch) (ch >= '0' && ch <= '9')
static int dispinfo_dev = -1;
static int canvas_w = 0, canvas_h = 0;
typedef struct size
{
  int w;
  int h;
} Size;
Size disp_size;
//********************************pa3**********************************
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  //********************************pa3**********************************
  struct timeval tv;
  gettimeofday(&tv, NULL);
  // 返回毫秒数
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
  //********************************pa3**********************************
}

int NDL_PollEvent(char *buf, int len) {
  //********************************pa3**********************************
  size_t read_len = read(evtdev, buf, len);
  return read_len;
  //********************************pa3**********************************
}



void NDL_OpenCanvas(int *w, int *h)
{
  if (*w == 0 && *h == 0)
  {
    *w = canvas_w;
    *h = canvas_h;
  }

  if (getenv("NWM_APP"))
  {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w;
    screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1)
    {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0)
        continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0)
        break;
    }

    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h)
{
  if (w == 0 && h == 0)
  {
    w = canvas_w;
    h = canvas_h;
  }
  assert(w > 0 && w <= canvas_w);
  assert(h > 0 && h <= canvas_h);
  for (size_t row = 0; row < h; ++row)
  {
    lseek(fbdev, x + (y + row) * canvas_w, SEEK_SET);
    write(fbdev, pixels + row * w, w);
  }
  write(fbdev, 0, 0);
}




int NDL_Init(uint32_t flags)
{
  if (getenv("NWM_APP"))
  {
    evtdev = 3;
  }
  //*****************************pa3*********************************
  // open 3 dev file
  evtdev = open("/dev/events", 0, 0);
  fbdev = open("/dev/fb", 0, 0);
  dispinfo_dev = open("/proc/dispinfo", 0, 0);

  char width_height[64];
  assert(read(dispinfo_dev, width_height, sizeof(width_height)));
  // 格式： 宽度,高度
  char *width = strtok(width_height, ",");
  char *height = width_height + strlen(width_height) + 1;
  sscanf(width, "%d", &canvas_w);
  sscanf(height, "%d", &canvas_h);
  printf("width:%d, height:%d\n", canvas_w, canvas_h);
  return 0;
  //*****************************pa3*********************************
}

void NDL_Quit()
{
  close(evtdev);
  close(dispinfo_dev);
}


void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}
