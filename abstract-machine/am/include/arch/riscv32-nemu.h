// #ifndef ARCH_H__
// #define ARCH_H__

// struct Context {
//   // TODO: fix the order of these members to match trap.S
//   //******************************************pa3***********************************************
//   uintptr_t x0; 
//   uintptr_t x1;
//   uintptr_t x2;
//   uintptr_t x3;
//   uintptr_t x4;
//   uintptr_t x5;
//   uintptr_t x6;
//   uintptr_t x7;
//   uintptr_t x8;
//   uintptr_t x9;
//   uintptr_t x10;
//   uintptr_t x11;
//   uintptr_t x12;
//   uintptr_t x13;
//   uintptr_t x14;
//   uintptr_t x15;
//   uintptr_t x16;
//   uintptr_t x17;
//   uintptr_t x18;
//   uintptr_t x19;
//   uintptr_t x20;
//   uintptr_t x21;
//   uintptr_t x22;
//   uintptr_t x23;
//   uintptr_t x24;
//   uintptr_t x25;
//   uintptr_t x26;
//   uintptr_t x27;
//   uintptr_t x28;
//   uintptr_t x29;
//   uintptr_t x30;
//   uintptr_t x31;
  

//   //******************************************pa3***********************************************
//   uintptr_t mepc, mcause, gpr[32], mstatus;
//   void *pdir;
// };

// #define GPR1 gpr[17] // a7
// #define GPR2 gpr[0]
// #define GPR3 gpr[0]
// #define GPR4 gpr[0]
// #define GPRx gpr[0]

// #endif

#ifndef ARCH_H__
#define ARCH_H__

struct Context
{
  // TODO: fix the order of these members to match trap.S
  uintptr_t gpr[32], mcause, mstatus, mepc, mscratch;
  void *pdir;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[10]
#define GPR3 gpr[11]
#define GPR4 gpr[12]
#define GPRx gpr[10]

#endif

