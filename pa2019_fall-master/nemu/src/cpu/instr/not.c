#include "cpu/instr.h"

make_instr_func(not_rm_v)
{
    int len=1;
    OPERAND reg;
    reg.data_size=data_size;
    len+=modrm_rm(eip+1,&reg);
    operand_read(&reg);
    reg.val=~reg.val;
    operand_write(&reg);
    print_asm_1("not","v",len,&reg);
    return len;
}