#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
  environ = empty;
  main(0, empty, empty);
  printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n");
  // exit(main(0, empty, empty));
  assert(0);
}
