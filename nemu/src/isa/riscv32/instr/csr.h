#include <isa.h>
#define CSR_REG(csr_num) (cpu.csr[csr2idx(csr_num)]._32)

static uint8_t csr2idx(uint32_t csr_num) {
    switch (csr_num) 
    {
        case MEPC_CODE : return MEPC_IDX;
        case MSTATUS_CODE : return MSTATUS_IDX;
        case MCAUSE_CODE : return MCAUSE_IDX;
        case MTVEC_CODE : return MTVEC_IDX;
        default: 
            panic("0x%x csr is not valid", csr_num);
            return -1;
    }
}

def_EHelper(csrrw) {
  // I:读后写控制状态寄存器
  // 记控制状态寄存器csr中的值为t，把rs1的结果写入csr，再把t的值写入rd

  // 获取特定csr寄存器的值，存入s0
  rtl_mv(s, s0, &CSR_REG(id_src2->imm));
  // 把rs1写入csr
  rtl_mv(s, &CSR_REG(id_src2->imm), dsrc1);
  // 再把t的值写入rd
  rtl_mv(s, ddest, s0);
}

#define MACHINE_SOFTWARE_INTERRUPT 11
def_EHelper(ecall) {
  // I:环境调用
  // 在状态为U情况下，将下一指令地址值保存在mepc寄存器，设置异常号于mcause寄存器中
  word_t trapvec = isa_raise_intr(MACHINE_SOFTWARE_INTERRUPT, s->snpc);
  // 从mtvec寄存器取出异常处理程序地址，跳转到该地址处执行异常处理
  rtl_j(s, trapvec);
}

def_EHelper(csrrs) {
  // I:读后置位控制状态寄存器
  // 记控制状态寄存器csr中的值为t，把t和rs1按位或的结果写入csr，再把t的值写入rd

  // 获取特定csr寄存器的值，存入s0
  rtl_mv(s, s0, &CSR_REG(id_src2->imm));
  // 把t和rs1按位或的结果写入csr
  rtl_or(s, &CSR_REG(id_src2->imm), s0, dsrc1);
  // 再把t的值写入rd
  rtl_mv(s, ddest, s0);
}

def_EHelper(mret) {
  // I:机器模式异常返回
  rtl_j(s, cpu.csr[MEPC_IDX]._32);
}


