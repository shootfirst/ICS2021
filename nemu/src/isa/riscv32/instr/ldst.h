def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

//***********************************pa2*************************************
def_EHelper(sh) {
  // R:把rs1-rs2半字写入rd
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 2);
}

def_EHelper(lbu) {
  // R:从rs1+立即数符号扩展读取一个字节，经过0扩展后写入ddest
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 1);
}

def_EHelper(lh) {
  // R:从rs1+立即数符号扩展读取半个字，经过符号扩展后写入ddest
  rtl_lms(s, ddest, dsrc1, id_src2->imm, 2);
}

def_EHelper(sb) {
  // R:从rs1+立即数符号扩展读取一个字节，经过0扩展后写入ddest
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 1);
}

def_EHelper(lhu) {
  // R:从rs1+立即数符号扩展读取半个字，经过0扩展后写入ddest
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 2);
}

def_EHelper(lb) {
  // R:从rs1+立即数符号扩展读取一个字节，经过符号扩展后写入ddest
  rtl_lms(s, ddest, dsrc1, id_src2->imm, 1);
}
//***********************************pa2*************************************
