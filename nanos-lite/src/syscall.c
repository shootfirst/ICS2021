#include <common.h>
#include "syscall.h"


//******************************pa3********************************
#define STD_IN 0    //标准输入
#define STD_OUT 1   //标准输出
#define STD_ERR 2   //错误输出
// enum {
//   SYS_exit,
//   SYS_yield,
//   SYS_open,
//   SYS_read,
//   SYS_write,
//   SYS_kill,
//   SYS_getpid,
//   SYS_close,
//   SYS_lseek,
//   SYS_brk,
//   SYS_fstat,
//   SYS_time,
//   SYS_signal,
//   SYS_execve,
//   SYS_fork,
//   SYS_link,
//   SYS_unlink,
//   SYS_wait,
//   SYS_times,
//   SYS_gettimeofday
// };

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

void sys_write(Context *c) {
  // 获取参数
  int fd = (int)c->GPR2;
  void *buf = (void*)c->GPR3;
  size_t count = (size_t)c->GPR4;

  
  switch (fd) {
    case STD_OUT ... STD_ERR:
      char *out = buf;
      for (int i = 0; i < count; ++i)
      {
        putch(out[i]);
      }
      break;
    default: panic("unknown fd = %d", fd);
  }
  c->GPRx = count;
}

void sys_brk(Context *c) {
  // 获取参数
  c->GPRx = 0;
}
//*****************************pa3**********************************

void do_syscall(Context *c) {
  uintptr_t a[4];
  // 从a7获取系统调用id
  a[0] = c->GPR1;
  // printf("%d\n", a[0]);

  switch (a[0]) {
    //*******************************pa3*************************************
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    case SYS_brk:
      sys_brk(c);
      break;
    //*******************************pa3*************************************
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
