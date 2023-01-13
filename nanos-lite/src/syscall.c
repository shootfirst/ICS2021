#include <common.h>
#include "syscall.h"

//******************************pa3********************************
#include "fs.h"
#include <sys/time.h>
//*****************************pa3***********************************

//******************************pa3********************************
// #define STD_IN 0    //标准输入
// #define STD_OUT 1   //标准输出
// #define STD_ERR 2   //错误输出
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

void sys_brk(Context *c) {
  c->GPRx = 0;
}

void sys_open(Context *c) {
  const char *path = (char*)c->GPR2;
  int flags = (int)c->GPR3;
  int mode = (int)c->GPR4;
  c->GPRx = fs_open(path, flags, mode);
}

void sys_read(Context *c) {
  int fd = (int)c->GPR2;
  void *buf = (void*)c->GPR3;
  size_t len = (size_t)c->GPR4;
  c->GPRx = fs_read(fd, buf, len);
}

void sys_write(Context *c) {
  int fd = (int)c->GPR2;
  const void *buf = (void*)c->GPR3;
  size_t len = (size_t)c->GPR4;
  c->GPRx = fs_write(fd, buf, len);
}

void sys_close(Context *c) {
  int fd = (int)c->GPR2;
  c->GPRx = fs_close(fd);
}

void sys_lseek(Context *c) {
  int fd = (int)c->GPR2;
  size_t offset = (size_t)c->GPR3; 
  int whence = (int)c->GPR4;
  c->GPRx = fs_lseek(fd, offset, whence);
}

void sys_gettimeofday(Context *c) {
  struct timeval *tv = (struct timeval*)c->GPR2;
  uint64_t t = io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec = t / 1000000;
  tv->tv_usec = t % 1000000;
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
    case SYS_open:
      sys_open(c);
      break;
    case SYS_read:
      sys_read(c);
      break;
    case SYS_close:
      sys_close(c);
      break;
    case SYS_lseek:
      sys_lseek(c);
      break;
    case SYS_gettimeofday:
      sys_gettimeofday(c);
      break;
    //*******************************pa3*************************************
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
