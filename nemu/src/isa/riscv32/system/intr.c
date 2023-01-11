#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  //***********************************pa3***********************************
  // 在状态为U情况下，将pc寄存器值保存在mepc寄存器，设置异常号于mcause寄存器中，返回从mtvec寄存器取出异常处理程序地址
  cpu.csr[MEPC_IDX]._32 = epc;
  cpu.csr[MCAUSE_IDX]._32 = NO;
  return cpu.csr[MTVEC_IDX]._32;
  //***********************************pa3***********************************

  return 0;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
