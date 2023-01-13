#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  //***********************************pa2***********************************
  // *cfg = (AM_GPU_CONFIG_T) {
  //   .present = true, .has_accel = false,
  //   .width = 0, .height = 0,
  //   .vmemsz = 0
  // };
  uint32_t info = inl(VGACTL_ADDR);
  uint16_t height = (uint16_t)(info & 0xFFFF);
  uint16_t width = (uint16_t)(info >> 16);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, .height = height,
    .vmemsz = 0
  };
  //***********************************pa2***********************************

}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  //***********************************pa2***********************************
  int win_weight = io_read(AM_GPU_CONFIG).width;  

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *pi = (uint32_t *)(uintptr_t)ctl->pixels;
  for (int i = 0; i < ctl->h; ++i){
    for (int j = 0; j < ctl->w; ++j){
      fb[(ctl->y) * win_weight + i * win_weight + ctl->x + j] = pi[i * (ctl->w) + j];
    }
  }
  //***********************************pa2***********************************
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}


