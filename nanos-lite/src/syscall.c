#include <common.h>
#include "syscall.h"


//******************************pa3********************************
void halt(int code);
void sys_exit(Context *c) {
  halt(c->GPR2);
}
void sys_yield(Context *c) {
  // 手动调用yield函数
  yield();
  // 返回值设为0
  c->GPRx = 0;
}
enum {
  SYS_exit,
  SYS_yield,
};
//*****************************pa3**********************************

void do_syscall(Context *c) {
  uintptr_t a[4];
  // 从a7获取系统调用id
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
