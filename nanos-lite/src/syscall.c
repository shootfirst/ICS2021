#include <common.h>
#include "syscall.h"

//******************************pa3********************************
void sys_exit(Context *c){
  // exit((int)c->GPR2);
  // c->GPRx = 0;
}
void sys_yield(Context *c){
  yield();
  c->GPRx = 0;
}
//*****************************pa3**********************************
enum {
  SYS_exit,
  SYS_yield,
};
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("%d\n", a[0]);

  switch (a[0]) {
    //*******************************pa3*************************************
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_yield:
      sys_yield(c);
      break;
    //*******************************pa3*************************************
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
