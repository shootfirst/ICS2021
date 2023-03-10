#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

//*******************************pa3**********************************
// https://five-embeddev.com/quickref/csrs.html
#define MEPC_CODE 0x341
#define MSTATUS_CODE 0x300
#define MCAUSE_CODE 0x342
#define MTVEC_CODE 0x305

#define MEPC_IDX 0
#define MSTATUS_IDX 1
#define MCAUSE_IDX 2
#define MTVEC_IDX 3
//*******************************pa3**********************************
typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];
  
  //*******************************pa3**********************************
  //csr寄存器
  // 0: mepc 1:mstatus 2:mcause 3:mtvec
  struct
  {
    rtlreg_t _32;
  } csr[4];
  //*******************************pa3**********************************

  vaddr_t pc;
} riscv32_CPU_state;

// decode
typedef struct {
  union {
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      int32_t  simm11_0  :12;
    } i;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm4_0    : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  simm11_5  : 7;
    } s;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm31_12  :20;
    } u;
    //************************************pa2**************************************
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm19_12  : 8;
      uint32_t imm11     : 1;
      uint32_t imm10_1   :10;
      uint32_t imm20     : 1;
    } j;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  funct7    : 7;
    } r;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm11     : 1;
      uint32_t imm4_1    : 4;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t  imm10_5  : 6;
      uint32_t  imm12    : 1;
    } b;
    //************************************pa2**************************************
    uint32_t val;
  } instr;
} riscv32_ISADecodeInfo;

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
