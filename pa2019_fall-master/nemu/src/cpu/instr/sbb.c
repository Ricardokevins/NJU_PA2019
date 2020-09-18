#include "cpu/instr.h"
make_instr_func(sbb_rm2r_v)
{
    int len=1;
    OPERAND reg, rm;//
    len+=modrm_r_rm(eip+1, &reg, &rm);
    rm.data_size=data_size;
    reg.data_size=data_size;
    operand_read(&reg);
    operand_read(&rm);
    //printf("add%d\n",rm.val);
    //printf("add%d\n",imm.val);
    reg.val=alu_sbb(rm.val,reg.val,data_size);
    operand_write(&reg);
    //printf("add%d\n",imm.val);
    //assert(0);
    print_asm_2("sbb","v",len,&rm,&reg);
    return len;
}