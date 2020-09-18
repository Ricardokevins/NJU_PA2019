#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(int_)
{
    //printf("hit inside\n");
    OPERAND imm;
    imm.sreg=SREG_CS;
    imm.data_size=8;
    
    imm.type=OPR_IMM;
    imm.addr=eip+1;
    
    operand_read(&imm);
    raise_sw_intr(imm.val);
    //printf("imm value %x\n",imm.val);
    print_asm_1("int","",2,&imm);
    
    return 0;
}