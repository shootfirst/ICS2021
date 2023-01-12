#include <stdint.h>
#include <stdio.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_yield 1
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  printf("ggg1\n");
  int ans = _syscall_(SYS_yield, 0, 0, 0);
  printf("ggg2\n");
  return ans;
  // return _syscall_(SYS_yield, 0, 0, 0);
}
