#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  return 1;
}

uint32_t SDL_GetTicks() {
  //***********************************pa3*****************************************
  return NDL_GetTicks();
  //***********************************pa3*****************************************
}

void SDL_Delay(uint32_t ms) {
  //***********************************pa3*****************************************
  uint32_t mtime = NDL_GetTicks();
  while( (NDL_GetTicks() - mtime) < ms){;}
  //***********************************pa3*****************************************
}
