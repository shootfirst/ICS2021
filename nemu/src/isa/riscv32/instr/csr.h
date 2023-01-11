#include <isa.h>
#define CSR_REG(csr_num) (cpu.csr[csr2idx(csr_num)]._32)

static uint8_t csr2idx(uint32_t csr_num) {
    switch (csr_num) 
    {
        case MEPC_ID : return MEPC_IDX;
        case MSTATUS_ID : return MSTATUS_IDX;
        case MCAUSE_ID : return MCAUSE_IDX;
        case MTVEC_ID : return MTVEC_IDX;
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
  // 把t和五位0扩展的立即数按位与的结结果写入csr
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

// def_EHelper(csrrs) {
//   // I:读后置位控制状态寄存器
//   // 记控制状态寄存器csr中的值为t，把t和五位0扩展的立即数按位与的结结果写入csr，再把t的值写入rd，csr的第5位及更高位保持不变（立即数只有5位）

//   // 获取特定csr寄存器的值，存入s0
//   rtl_mv(s, s0, &CSR_REG(id_src2->imm));
//   // 把t和五位0扩展的立即数按位与的结结果写入csr
//   rtl_or(s, &CSR_REG(id_src2->imm), s0, dsrc1);
//   // 再把t的值写入rd
//   rtl_mv(s, ddest, s0);
// }

// #include <isa.h>
// static uint8_t csr_id_instr2array(uint32_t csr_num) {
//     switch (csr_num) 
//     {
//         case MEPC_ID : return MEPC_IDX;
//         case MSTATUS_ID : return MSTATUS_IDX;
//         case MCAUSE_ID : return MCAUSE_IDX;
//         case MTVEC_ID : return MTVEC_IDX;
//         default: 
//             panic("0x%x csr is not valid", csr_num);
//             return -1;
//     }
// }

// void difftest_skip_ref();
// #define csr(idx) (cpu.csr[csr_id_instr2array(idx)]._32)
// def_EHelper(csrrw)
// {
//   // difftest_skip_ref();
//   // Log("csrrw register %x\n", id_src2->imm);
//   rtl_mv(s, s0, &csr(id_src2->imm));
//   rtl_mv(s, &csr(id_src2->imm), id_src1->preg);
//   rtl_mv(s, id_dest->preg, s0);
// }

// def_EHelper(ecall)
// {
//   bool success = false;
//   word_t trap_no = isa_reg_str2val("$a7", &success);
//   // Log("ecal");
//   // Log("trap number is %d\n",trap_no);
//   // Log("trap number is %d\n", trap_no);
//   if (!success)
//     Assert(0, "Invalid gpr register!");
//   word_t trap_vec = isa_raise_intr(trap_no, s->snpc);
//   rtl_j(s, trap_vec);
// }