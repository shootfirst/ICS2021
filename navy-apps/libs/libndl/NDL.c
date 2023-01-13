#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//********************************pa3**********************************
#include <sys/time.h>
#include <fcntl.h>
#include <assert.h>
static int canvas_x = 0, canvas_y = 0;
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
  int fd = open("/dev/events", 0, 0);
  size_t read_len = read(fd, buf, len);
  close(fd);
  return read_len;
  //********************************pa3**********************************
}

void NDL_OpenCanvas(int *w, int *h) {
  //*********************************pa3**************************************
  // 画布大小不能超出屏幕大小
  if (*w == 0){
    *w = screen_w;
  }else if(*w > screen_w){
    assert(0);
  }
  if (*h == 0){
    *h = screen_h;
  }else if(*h > screen_h){
    assert(0);
  }
  //*********************************pa3**************************************
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  //*************************************pa3*****************************************
  if (w == 0 && h == 0) {
    w = screen_w;
    h = screen_h;
  }

  int graphics = open("/dev/fb", O_RDWR);
  
  for (int i = 0; i < h; ++i){
    lseek(graphics, ((canvas_y + y + i) * screen_w + (canvas_x + x)) * sizeof(uint32_t), SEEK_SET);
    ssize_t s = write(graphics, pixels + w * i, w * sizeof(uint32_t));
  }
  //*************************************pa3*****************************************
}




int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  //*****************************pa3*********************************
  char width_height[64];
  int fd = open("/proc/dispinfo", 0, 0);
  assert(read(fd, width_height, sizeof(width_height)));
  // 格式： 宽度,高度
  char *width = strtok(width_height, ",");
  char *height = width_height + strlen(width_height) + 1;
  sscanf(width, "%d", &screen_w);
  sscanf(height, "%d", &screen_h);
  printf("width:%d, height:%d\n", screen_w, screen_h);
  //*****************************pa3*********************************
  return 0;
}

void NDL_Quit() {
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
