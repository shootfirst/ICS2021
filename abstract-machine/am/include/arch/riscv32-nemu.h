#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  // TODO: fix the order of these members to match trap.S
  //******************************************pa3***********************************************
  uintptr_t gpr[32], mepc, mcause, mstatus;
  //******************************************pa3***********************************************
  void *pdir;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif

// #ifndef ARCH_H__
// #define ARCH_H__

// struct Context
// {
//   // TODO: fix the order of these members to match trap.S
//   uintptr_t gpr[32], mcause, mstatus, mepc;
//   void *pdir;
// };

// #define GPR1 gpr[17] // a7
// #define GPR2 gpr[10]
// #define GPR3 gpr[11]
// #define GPR4 gpr[12]
// #define GPRx gpr[10]

// #endif

