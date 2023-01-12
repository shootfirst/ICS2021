#include <common.h>

//*******************************pa3*************************************
void do_syscall(Context *c);
//*******************************pa3*************************************

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    //********************************pa3*********************************
    case EVENT_YIELD: 
      printf("yyyyiiiieeeelllldddd\n");
      break;
    
    case EVENT_SYSCALL:
      do_syscall(c);
      break;
    //********************************pa3*********************************
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
