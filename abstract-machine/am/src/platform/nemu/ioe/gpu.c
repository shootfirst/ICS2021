#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

//*************************************pa3*******************************************
#define SIZE_ADDR (VGACTL_ADDR)
//*************************************pa3*******************************************

void __am_gpu_init() {
  // int i;
  // int w = inw(SIZE_ADDR+2);  // TODO: get the correct width
  // int h = inw(SIZE_ADDR);  // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
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
//       // printf("write pixels[%d] of %d to fb[%d, %d]\n", p_pos, p[p_pos], row , x);
//       p_pos += w;
//     }
//   }
//   if (ctl->sync) {
//     outl(SYNC_ADDR, 1);
//   }
// }

// void __am_gpu_status(AM_GPU_STATUS_T *status) {
//   status->ready = true;
// }
