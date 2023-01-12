#include <cpu/decode.h>
#include "../local-include/rtl.h"


#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(addi) f(auipc) f(jal) f(jalr) \
                f(add) f(sltu) f(xor) f(or) f(sltiu) f(beq) f(bne) \
                f(sub) f(sh) \
                f(srai) f(lbu) f(andi) f(sll) f(and) f(xori) \
                f(lh) f(lhu) f(lb) f(sb) \
                f(bge) \
                f(mul) f(div) \
                f(rem) \
                f(blt) f(slt) \
                f(slli) f(srli) \
                f(mulh) \
                f(sra) f(srl) \
                f(bltu) \
                f(divu) \
                f(bgeu) f(ori) \
                f(csrrw) f(ecall) f(csrrs) f(mret) f(remu)

def_all_EXEC_ID();
