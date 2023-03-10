def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

//***********************************pa2*************************************
// 符号位扩展宏
#define sym_ext(a,num) (((sword_t)a<<num)>>num)

def_EHelper(addi) {
  // I:把符号位扩展的立即数和rs1相加写入rd
  rtl_addi(s, ddest, dsrc1, sym_ext(id_src2->imm, 20));
}

def_EHelper(auipc) {
  // U:把符号位扩展的立即数（左移12位，已经实现了左移）和pc相加写入目的寄存器
  rtl_addi(s, ddest, &s->pc, sym_ext(id_src1->imm, 0));
}

def_EHelper(jal) {
  // J:将pc+4的值放入目的寄存器，将pc值设为当前值+符号位扩展的立即数
  rtl_addi(s, ddest, &s->pc, 4);
  rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src1->imm, 11));
}

def_EHelper(jalr) {
  // I:把pc+4写入rd，把pc设为rs1+符号位扩展的立即数，并且把最低位设为1
  rtl_addi(s, s0, &s->pc, 4);
  rtl_addi(s, &s->dnpc, dsrc1, sym_ext(id_src2->imm, 20));
  rtl_andi(s, &s->dnpc, &s->dnpc, ~1);
  rtl_addi(s, ddest, s0, 0);
}

def_EHelper(add) {
  // R:把rs1+rs2写入rd
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  // R:比较rs1和rs2，比较时视为无符号数，如果rs1小则向rd写入1，否则写入0
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}

def_EHelper(xor) {
  // R:把rs1^rs2写入rd
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  // R:把rs1 | rs2写入rd
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltiu) {
  // I:把rs1和和符号位扩展的立即数比较，比较时视为无符号数，如果rs1更小，向rd写入1，否则写入0
  rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, sym_ext(id_src2->imm, 20));
}

def_EHelper(beq) {
  // B:若rs1和rs2值相等，将pc值加上立即数的符号位扩展
  if (*dsrc1 == *ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}

def_EHelper(bne) {
  // B:若rs1和rs2值不相等，将pc值加上立即数的符号位扩展
  if (*dsrc1 != *ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}

def_EHelper(sub) {
  // R:把rs1-rs2写入rd
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srai) {
  // I:把rs1右移立即数位，空位用rs1最高位填充填入rd
  rtl_srai(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(andi) {
  // I:把符号位扩展的立即数和rs1相&&写入rd
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(sll) {
  // R:把rs1<<rs2,空出部分填0写入rd，rs2低5位代表移动位数
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  // R:把rs1&rs2写入rd
  rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(xori) {
  // I:把rs1^立即数位，空位用rs1最高位填充填入rd
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(bge) {
  // B:若rs1>=rs2值（视为二进制补码），将pc值加上立即数的符号位扩展
  if ((sword_t)*dsrc1 >= (sword_t)*ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}

def_EHelper(mul) {
  // R:把rs1 * rs2写入rd
   rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div) {
  // R:把rs1 / rs2写入rd
   rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
  // R:把rs1 % rs2写入rd
   rtl_divs_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(blt) {
  // B:若rs1<rs2值（视为二进制补码），将pc值加上立即数的符号位扩展
  if ((sword_t)*dsrc1 < (sword_t)*ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}

def_EHelper(slt) {
  // R:若rs1 < rs2，则写入rd 1，否则写入0
   rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}

def_EHelper(slli) {
  // I:把rs1左移立即数位写入rd，空出部分填0，对于riscv32，只有立即数的第6位（最高位）为0时，指令才有效
  rtl_slli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}

def_EHelper(srli) {
  // I:把rs1左移立即数位写入rd，空出部分填0，对于riscv32，只有立即数的第6位（最高位）为0时，指令才有效
  rtl_srli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}

def_EHelper(mulh) {
  // R:把rs1 * rs2高位写入rd
   rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  // R:把rs1右移rs2位写入rd，空位用rs1最高位填充
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  // R:把rs1右移rs2位写入rd，空位填0
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(bltu) {
  // B:若rs1<rs2值，将pc值加上立即数的符号位扩展
  if (*dsrc1 < *ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}

def_EHelper(divu) {
  // R:把rs1 / rs2写入rd，二者视作无符号数
   rtl_divu_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(bgeu) {
  // B:若rs1>=rs2值，将pc值加上立即数的符号位扩展
  if (*dsrc1 >= *ddest) {
    rtl_addi(s, &s->dnpc, &s->pc, sym_ext(id_src2->imm, 19));
  }
}


def_EHelper(ori) {
   // I:把符号位扩展的立即数和rs1相||写入rd
  rtl_ori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(remu) {
  // R:把rs1 % rs2(看成无符号数)写入rd
  rtl_divu_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulhu) {
  // R:把rs1 * rs2高位写入rd(无符号乘)
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}



//***********************************pa2*************************************


