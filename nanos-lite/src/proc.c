#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

//*********************************pa3**********************************
void naive_uload(PCB *pcb, const char *filename);
//*********************************pa3**********************************

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  //******************************pa3*********************************
  naive_uload(NULL, "/bin/nterm");
  //******************************pa3*********************************
}

Context* schedule(Context *prev) {
  return NULL;
}
