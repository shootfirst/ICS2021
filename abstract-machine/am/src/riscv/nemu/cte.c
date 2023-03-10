#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

//*********************************pa3*************************************
#define MACHINE_SOFTWARE_INTERRUPT 11
//*********************************pa3*************************************

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      //*********************************pa3*************************************
      // 由ecall指引起的trap
      case MACHINE_SOFTWARE_INTERRUPT:
        // 若a7寄存器值为-1，表示yield
        if (c->GPR1 == -1) { 
          ev.event = EVENT_YIELD;
        // 否则是系统调用
        } else {
          ev.event = EVENT_SYSCALL;
        }
        break;
      //*********************************pa3*************************************
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
