#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    //********************************pa3*********************************
    case EVENT_YIELD: 
      printf("yes, yield\n");
      break;
    
    case EVENT_SYSCALL:
      printf("yes, syscall\n");
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
