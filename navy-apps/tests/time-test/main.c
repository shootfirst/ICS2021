//******************************************pa3********************************************
#include <stdio.h>
#include <NDL.h>

int main() {
  NDL_Init(0);
  uint32_t mtime = 500;
  uint32_t time_inteval = 0;
  while (1) {
      while (time_inteval < mtime) {
          time_inteval = NDL_GetTicks();
      }
      mtime += 500;
      printf("pass 0.5 s\n");
  }

  NDL_Quit();
}
//******************************************pa3********************************************
