#include <isa.h>
#define CSR_REG(csr_num) (cpu.csr[csr2idx(csr_num)]._32)

static uint8_t csr2idx(uint32_t csr_num) {
    switch (csr_num) 
    {
        case 0x305 : return 0;
        case 0x342 : return 1;
        case 0x300 : return 2;
        case 0x341 : return 3;
        case 0x180 : return 4;
        case 0x340 : return 5;
        default: 
            panic("0x%x csr is not valid", csr_num);
            return -1;
    }
}

def_EHelper(csrrs) {
  // I:读后置位控制状态寄存器
  // 记控制状态寄存器csr中的值为t，把t和五位0扩展的立即数按位与的结结果写入csr，再把t的值写入rd，csr的第5位及更高位保持不变（立即数只有5位）

  // 获取特定csr寄存器的值，存入s0
  rtl_mv(s, s0, &CSR_REG(id_src2->imm));
  // 把t和五位0扩展的立即数按位与的结结果写入csr
  rtl_or(s, &CSR_REG(id_src2->imm), s0, dsrc1);
  // 再把t的值写入rd
  rtl_mv(s, ddest, s0);
}