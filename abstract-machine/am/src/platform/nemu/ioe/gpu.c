#include <am.h>
#include <nemu.h>
//***********************************pa2***********************************
#include <klib.h>
//***********************************pa2***********************************

//***********************************pa2***********************************
#define SIZE_ADDR (VGACTL_ADDR)
//***********************************pa2***********************************

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  //***********************************pa2***********************************
  int i;
  // 获取宽度（从vga.c的初始化文件中可以得知）
  int w = inw(SIZE_ADDR + 2);  
  // 获取高度
  int h = inw(SIZE_ADDR);  
  // 获取显存起始地址
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // 向显存写入内容
  for (i = 0; i < w * h; i++) fb[i] = i;
  // 向同步寄存器写入1，让其更新屏幕
  outl(SYNC_ADDR, 1);
  //***********************************pa2***********************************
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  //***********************************pa2***********************************
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    // 填写显示器宽度与高度
    .width = inw(SIZE_ADDR + 2), .height = inw(SIZE_ADDR),
    .vmemsz = 0
  };
  //***********************************pa2***********************************
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  //***********************************pa2***********************************
  // 向从坐标(ctl->x, ctl->y)开始，绘制ctl->w * ctl->h的矩形图像. 图像像素按行优先方式存储在ctl->pixels中
  // 若ctl->sync为true, 则马上将帧缓冲中的内容同步到屏幕上，绘制到屏幕的方法就是将像素ctl->pixels拷贝到显存FB_ADDR指定位置
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  int width = io_read(AM_GPU_CONFIG).width;
  // 获取像素
  uint32_t *pixels = ctl->pixels;
  // 获取显存
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // 防止越界！！！
  uint32_t len = sizeof(uint32_t) * (w < width - x ? w : width - x);
  int pos_pixels = 0;
  for(int j = y; j < y + h; j++, pos_pixels += w){
    // 二维数组类似fb + j * width + x
    memcpy(fb + j * width + x, pixels + (pos_pixels), len);
  }
  // int win_weight = io_read(AM_GPU_CONFIG).width;  
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // uint32_t *pi = (uint32_t *)(uintptr_t)ctl->pixels;
  // for (int i = 0; i < ctl->h; ++i){
  //   for (int j = 0; j < ctl->w; ++j){
  //     fb[(ctl->y) * win_weight + i * win_weight + ctl->x + j] = pi[i * (ctl->w) + j];
  //   }
  // }
  //***********************************pa2***********************************
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

// void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
//   if(ctl->w != 0 && ctl->h != 0){
//     int W = inw(SIZE_ADDR+2);  
//     int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
//     uint32_t* p = ctl->pixels;

//     uint32_t p_pos = 0;
//     uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
//     uint32_t cp_byte = sizeof(uint32_t) * min(w , W - x);
//     for(int row = y; row < y + h; ++ row){
//       memcpy(fb + x + row * W, p + (p_pos), cp_byte);
//       p_pos += w;
//     }
//   }
//   if (ctl->sync) {
//     outl(SYNC_ADDR, 1);
//   }
// }

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}


// #include <am.h>
// #include <nemu.h>
// #include <klib.h>

// #define SYNC_ADDR (VGACTL_ADDR + 4)
// #define SIZE_ADDR (VGACTL_ADDR)

// #define min(x,y) ( (x < y) ? x : y )


// void __am_gpu_init() {
//   int i;
//   int w = inw(SIZE_ADDR+2);  // TODO: get the correct width
//   int h = inw(SIZE_ADDR);  // TODO: get the correct height
//   uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
//   for (i = 0; i < w * h; i ++) fb[i] = i;
//   outl(SYNC_ADDR, 1);
// }

// void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
//   *cfg = (AM_GPU_CONFIG_T) {
//     .present = true, .has_accel = false,
//     .width = inw(SIZE_ADDR+2), .height = inw(SIZE_ADDR),
//     .vmemsz = 0
//   };
// }



// void __am_gpu_status(AM_GPU_STATUS_T *status) {
//   status->ready = true;
// }
